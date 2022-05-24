package fi.tuni.prog3.sisu;

import java.io.FileNotFoundException;
import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import javafx.application.Platform;

import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.ListView;
import javafx.scene.control.TreeItem;

import javafx.scene.control.TreeTableColumn;
import javafx.scene.control.TreeTableView;
import javafx.scene.web.WebView;

/**
 * Handles user i/o of the course view.
 */
public class CoursesController extends BaseController implements Initializable {

  /**
   * Default constructor.
   * @param name Name of the controller.
   * @param fxml Filename of the according .fxml file.
   * @param sisu The singleton SisuLogic object.
   */
  public CoursesController(String name, String fxml, SisuLogic sisu) {
        super(name, fxml, sisu);
    }

    @FXML
    private Button cancelButton;

    @FXML
    private TreeTableView<DegreePlanItem> degreeTree;

    @FXML
    private TreeTableColumn<DegreePlanItem, String> colOne;

    @FXML
    private ListView<DegreeCourse> courseListView;

    @FXML
    private Button saveButton;

    @FXML
    private WebView courseInfo;

    @FXML
    private Label courseNameLbl;

    @FXML
    private Label pointsLbl;

    @FXML
    private Label statusMessage;

    @FXML
    private WebView outcomes;

    @FXML
    void cancelActioln(ActionEvent event) {
        observableList.clear();
        initData();
    }

    @FXML
    void saveAction(ActionEvent event) throws FileNotFoundException {
        getSisu().saveSelection();
    }

    private final ObservableList<DegreeCourse> observableList = FXCollections.observableArrayList();

  /**
   * Initializes the view.
   * @param location
   * @param resources
   */
  @Override
    public void initialize(URL location, ResourceBundle resources) {
        statusMessage.setText("Ladataan tietoja. Tämä saattaa kestää minuutteja");

        initData();

        // Fires only if user degree selection changes
        getSisu().addListener(() -> {
            statusMessage.setText("Ladataan tietoja. Tämä saattaa kestää minuutteja");
            courseListView.getSelectionModel().clearSelection();
            observableList.clear();
            degreeTree.setRoot(null);
            updateCourseDetailsInfomration(null);
            initData();
        });

        colOne.setCellValueFactory(value -> {
            if (value == null) {
                return new ReadOnlyObjectWrapper<>("");
            }
            TreeItem<DegreePlanItem> item = value.getValue();
            int points = item.getValue().getPoints();
            String message = item.getValue().getName();
            if (points > 0) {
                message += " " + points + " op";
            }
            return new ReadOnlyObjectWrapper<>(message);
        });

        this.degreeTree.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends TreeItem<DegreePlanItem>> observable, TreeItem<DegreePlanItem> oldValue, TreeItem<DegreePlanItem> newValue) -> {
            observableList.clear();
            updateCourseDetailsInfomration(null);
            if (newValue != null) {
                observableList.addAll(newValue.getValue().getDegreeCourses());
                courseListView.setItems(observableList);
            }

        });

        courseListView.setCellFactory(new CourseCellFactory());

        courseListView.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends DegreeCourse> ov, DegreeCourse old_val, DegreeCourse new_val) -> {
            updateCourseDetailsInfomration(new_val);

        });

    }

    private void initData() {
        User user = getSisu().getActiveUser();
        if (user == null) {
            return;
        }
        Thread one = new Thread() {
            @Override
            public void run() {
                System.out.println("Ladataan");
                DegreePlanItem planRoot = getSisu().fetchDegreePlan(user);

                Platform.runLater(() -> {
                    statusMessage.setText("");
                    TreeItem<DegreePlanItem> root = new TreeItem<>(planRoot);

                    buildTreeTableItems(planRoot.getDegreePlans(), root);
                    degreeTree.setRoot(root);
                });

            }
        };
        one.start();
    }

    private void buildTreeTableItems(ArrayList<DegreePlanItem> items, TreeItem<DegreePlanItem> treeItem) {

        for (DegreePlanItem item : items) {
            TreeItem<DegreePlanItem> newTreeItem = new TreeItem<>(item);
            treeItem.getChildren().add(newTreeItem);
            buildTreeTableItems(item.getDegreePlans(), newTreeItem);
        }

    }

    private void updateCourseDetailsInfomration(DegreeCourse new_val) {
        if (new_val == null) {
            this.courseNameLbl.setText("");
            this.pointsLbl.setText("");
            this.courseInfo.getEngine().loadContent("", "text/html");
            this.outcomes.getEngine().loadContent("", "text/html");
        } else {
            this.courseNameLbl.setText(new_val.getName());
            this.pointsLbl.setText("Opintopisteet: " + new_val.getPoints());
            this.courseInfo.getEngine().loadContent(new_val.getDescription(false), "text/html");
            this.outcomes.getEngine().loadContent(new_val.getOutcomes(false), "text/html");

        }

    }

}
