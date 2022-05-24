package fi.tuni.prog3.sisu;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.beans.property.SimpleObjectProperty;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TableColumn;
import javafx.scene.control.TablePosition;
import javafx.scene.control.TableRow;
import javafx.scene.control.TableView;
import javafx.scene.control.cell.ComboBoxTableCell;
import javafx.scene.control.cell.PropertyValueFactory;

/**
 * Class to control the teacher view
 */
public class TeacherController extends BaseController implements Initializable {

    public TeacherController(String name, String fxml, SisuLogic sisu) {
        super(name, fxml, sisu);
    }

    @FXML
    private TableColumn<DegreeCourse, String> PointCol;

    @FXML
    private TableColumn<DegreeCourse, String> nameCol;

    @FXML
    private TableColumn<DegreeCourse, DegreeCourse.Grade> gradeCol;

    @FXML
    private ListView<User> studentListView;

    @FXML
    private TableView<DegreeCourse> studentCoursesTableView;

    @FXML
    /**
     * Saves grades to json file
     * @param event Clicking of the save button
     * @throws FileNotFoundException If no save file is found
     */
    void saveGrades(ActionEvent event) throws FileNotFoundException {
        this.getSisu().saveSelection();
    }

    private final ObservableList<User> observableList = FXCollections.observableArrayList();

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        observableList.setAll(getSisu().getUsers());
        initListView();
        initTableView();
        super.getSisu().addListener(() -> {
            //updateView();
        });
    }

    private void initListView() {
        studentListView.setCellFactory((ListView<User> param) -> new ListCell<>() {
            @Override
            public void updateItem(User user, boolean empty) {
                super.updateItem(user, empty);
                if (empty || user == null) {
                    setText(null);
                } else {
                    setText(user.toString());
                }
            }
        });

        studentListView.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends User> observable, User oldValue, User newValue) -> {
            studentCoursesTableView.getItems().clear();
            if (newValue != null) {
                this.getSisu().setActiveUser(newValue);
                DegreePlanItem plan;
                try {
                    plan = this.getSisu().fetchDegreePlanFromFile(newValue);
                    ArrayList<DegreeCourse> courses = new ArrayList<>();
                    this.buildCourseItems(plan.getDegreePlans(), courses);
                    studentCoursesTableView.getItems().setAll(courses);
                } catch (IOException ex) {
                    Logger.getLogger(TeacherController.class.getName()).log(Level.INFO, "No degree saved yet");
                }
            }

        });

        studentListView.setItems(observableList);
    }

    private void initTableView() {
        studentCoursesTableView.setEditable(true);
        PointCol.setCellValueFactory(new PropertyValueFactory<DegreeCourse, String>("points"));
        nameCol.setCellValueFactory(new PropertyValueFactory<DegreeCourse, String>("name"));

        gradeCol.setCellValueFactory((TableColumn.CellDataFeatures<DegreeCourse, DegreeCourse.Grade> param) -> {
            DegreeCourse degreeCourse = param.getValue();
            DegreeCourse.Grade grade = DegreeCourse.Grade.getByCode(degreeCourse.getGrade());
            return new SimpleObjectProperty<DegreeCourse.Grade>(grade);
        });

        gradeCol.setOnEditCommit((TableColumn.CellEditEvent<DegreeCourse, DegreeCourse.Grade> event) -> {
            TablePosition<DegreeCourse, DegreeCourse.Grade> pos = event.getTablePosition();
            DegreeCourse.Grade newGrade = event.getNewValue();
            int row = pos.getRow();
            DegreeCourse degree = event.getTableView().getItems().get(row);
            degree.setGrade(newGrade.getCode());
        });

        gradeCol.setCellFactory(ComboBoxTableCell.forTableColumn(DegreeCourse.Grade.values()));
        studentCoursesTableView.setRowFactory(tv -> new TableRow<DegreeCourse>() {
            @Override
            protected void updateItem(DegreeCourse item, boolean empty) {
                super.updateItem(item, empty);
                /* if (item == null) {
                    setStyle("");
                } else if (item.getGrade() > 0) {
                    setStyle("-fx-background-color: #baffba;");
                } else if (item.getGrade() == 0) {
                    setStyle("-fx-background-color: #ffd7d1;");
                } else {
                    setStyle("");
                }*/
            }
        });
    }

    private void updateView() {
        observableList.clear();
        observableList.setAll(getSisu().getUsers());
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
