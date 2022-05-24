package fi.tuni.prog3.sisu;

import fi.tuni.prog3.sisu.DegreeCourse.Grade;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.beans.property.SimpleObjectProperty;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Label;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TableColumn.CellDataFeatures;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.ComboBoxTableCell;
import javafx.scene.control.cell.PropertyValueFactory;

/**
 * Class to control progress window
 * @author markus
 */
public class ProgressController extends BaseController implements Initializable {

    public ProgressController(String name, String fxml, SisuLogic sisu) {
        super(name, fxml, sisu);
    }

    @FXML
    private TableColumn<DegreeCourse, Integer> PointCol;

    @FXML
    private TableColumn<DegreeCourse, String> nameCol;

    @FXML
    private TableColumn<DegreeCourse, Grade> gradeCol;

    @FXML
    private TableView<DegreeCourse> studentCoursesTableView;
    @FXML
    private Label pointsLbl;

    @FXML
    private Label pointsLeft;

    @FXML
    private Label currentPoints;

    @FXML
    private Label avgLbl;

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        initTableView();
        initListView();

        super.getSisu().addListener(() -> {
            System.out.println("Update progress view");
            updateView();
        });
    }

    private void initListView() {
        DegreePlanItem plan = getSisu().getRoot();
        if (plan == null) {
            try {
                plan = getSisu().fetchDegreePlanFromFile(getSisu().getActiveUser());
            } catch (IOException ex) {
                Logger.getLogger(ProgressController.class.getName()).log(Level.INFO, "Degree not found yet");
                return;
            }

        }

        ArrayList<DegreeCourse> courses = new ArrayList<>();
        this.buildCourseItems(plan.getDegreePlans(), courses);
        studentCoursesTableView.getItems().setAll(courses);
        calculateStatics();

    }

    private void calculateStatics() {
      try{
        int total = studentCoursesTableView.getItems().stream().mapToInt(integer -> Integer.valueOf(integer.getPoints())).sum();
        int current = studentCoursesTableView.getItems().stream().mapToInt(integer -> {
            System.out.println(integer.getGrade());
            if (!integer.getGrade().equals("6")) {
                return Integer.valueOf(integer.getPoints());
            }
            return 0;
        }
        ).sum();

        long grades = studentCoursesTableView.getItems().stream().mapToInt(integer -> {
            if (!integer.getGrade().equals("6")) {
                return Integer.valueOf(integer.getGrade());
            }
            return 0;
        }
        ).sum();

        long count = studentCoursesTableView.getItems().stream().filter((x) -> !x.getGrade().equals("6")).count();

        if (count > 0) {
            this.avgLbl.setText(String.format("Keskiarvo: %.2f", (grades / (double) count)));
        }
        this.pointsLbl.setText("Opintopisteet: " + String.valueOf(total));
        this.currentPoints.setText("Suoritettu: " + String.valueOf(current));
        this.pointsLeft.setText("Jäljellä: " + String.valueOf(total - current));
      }
      catch(NumberFormatException e){
        System.out.println("Couldn't calculate total credits");
      }

    }

    private void initTableView() {
        PointCol.setCellValueFactory(new PropertyValueFactory<DegreeCourse, Integer>("points"));
        nameCol.setCellValueFactory(new PropertyValueFactory<DegreeCourse, String>("name"));

        gradeCol.setCellValueFactory((CellDataFeatures<DegreeCourse, Grade> param) -> {
            DegreeCourse degreeCourse = param.getValue();
            Grade grade = Grade.getByCode(degreeCourse.getGrade());
            return new SimpleObjectProperty<Grade>(grade);
        });

        gradeCol.setCellFactory(ComboBoxTableCell.forTableColumn(DegreeCourse.Grade.values()));
        studentCoursesTableView.setRowFactory(tv -> new TableRow<DegreeCourse>() {
            @Override
            protected void updateItem(DegreeCourse item, boolean empty) {
                super.updateItem(item, empty);
                if (item == null) {
                    setStyle("");
                } else if (Grade.getByCode(item.getGrade()) == Grade.FIVE) {
                    setStyle("-fx-background-color: #baffba;");
                } else if (Grade.getByCode(item.getGrade()) == Grade.ZERO) {
                    setStyle("-fx-background-color: #ffd7d1;");
                } else {
                    setStyle("");
                }
            }
        });
    }

    private void updateView() {
        studentCoursesTableView.getItems().clear();
        initListView();
    }

    private void buildCourseItems(ArrayList<DegreePlanItem> items, ArrayList<DegreeCourse> treeItems) {
        for (DegreePlanItem item : items) {
            for (DegreeCourse course : item.getDegreeCourses()) {
                if (course.isSelected()) {
                    treeItems.add(course);
                }
            }
            buildCourseItems(item.getDegreePlans(), treeItems);
        }

    }

}
