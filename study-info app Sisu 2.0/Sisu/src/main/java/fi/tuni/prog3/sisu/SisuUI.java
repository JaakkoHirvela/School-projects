package fi.tuni.prog3.sisu;

import fi.tuni.prog3.sisu.SisuLogic.Mode;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.ResourceBundle;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.VBox;
import javafx.stage.Stage;

/**
 * Class to control the first window
 */
public class SisuUI implements Initializable {

    private BorderPane root;
    private SisuLogic sisu;

    private HashMap<OpenView, Node> views = new HashMap<OpenView, Node>();

    enum OpenView {
        COURSE_VIEW,
        USER_INFO,
        TEACHER_VIEW, PROGRESS_VIEW
    }

    /**
     * Constructor
     * @param sisu the SisuLogic object
     * @throws IOException
     */
    public SisuUI(SisuLogic sisu) throws IOException {
        this.sisu = sisu;

    }
    @FXML
    private Label modeLbl;

    @FXML
    private Button coursesButton;

    @FXML
    private Button profileButton;

    @FXML
    private Button teacherButton;

    @FXML
    private Button progressBtn;

    @FXML
    private Label currentUserLbl;

    @FXML
    private VBox leftSide;

    @FXML
    private AnchorPane mesageLbl;

    @FXML
    void openCourses(ActionEvent event) {
        initView(OpenView.COURSE_VIEW);
    }

    @FXML
    void openProfile(ActionEvent event) {
        initView(OpenView.USER_INFO);
    }

    @FXML
    void openTeacher(ActionEvent event) {
        initView(OpenView.TEACHER_VIEW);
    }

    @FXML
    void openProgress(ActionEvent event) {
        initView(OpenView.PROGRESS_VIEW);
    }

    void showLogin() {
        LoginViewController loginViewController = new LoginViewController("loginView", "loginView.fxml", this.sisu);
        loginViewController.show();
    }

    private void initView(OpenView openView) {
        try {
            if (views.containsKey(openView)) {
                root.setCenter(views.get(openView));
            } else {
                BaseController controller = getController(openView);
                FXMLLoader loader = new FXMLLoader(getClass().getResource(controller.getFxml()));
                loader.setController(controller);
                AnchorPane pane = loader.load();
                views.put(openView, pane);
                root.setCenter(pane);
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private BaseController getController(OpenView openView) {
        switch (openView) {
            case COURSE_VIEW:
                return new CoursesController("CoursesView", "CoursesView.fxml", this.sisu);
            case USER_INFO:
                return new UserInfoController("UserInfo", "UserInfo.fxml", this.sisu);
            case TEACHER_VIEW:
                return new TeacherController("TeacherView", "TeacherView.fxml", this.sisu);
            case PROGRESS_VIEW:
                return new ProgressController("ProgressView", "ProgressView.fxml", this.sisu);
        }
        return null;
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        showLogin();
        currentUserLbl.setText("");
        if (sisu.getActiveUser() != null) {
            currentUserLbl.setText("Käyttäjä: " + sisu.getActiveUser());
        }
        modeLbl.setText("Mode: " + String.valueOf(sisu.getMode()));
        sisu.addListener(() -> {
            if (this.sisu.getMode() == Mode.STUDENT) {
                currentUserLbl.setText("Käyttäjä: " + sisu.getActiveUser());
            }

            if (this.sisu.getActiveUser().getDegreeId() == null || this.sisu.getActiveUser().getDegreeId().isEmpty()) {
                coursesButton.setDisable(true);

            } else {
                coursesButton.setDisable(false);
            }

            if (this.sisu.getRoot() != null) {
                progressBtn.setDisable(false);
            } else {
                progressBtn.setDisable(true);
            }

        });

        Mode mode = this.sisu.getMode();
        switch (mode) {
            case TEACHER:
                leftSide.getChildren().remove(coursesButton);
                leftSide.getChildren().remove(profileButton);
                leftSide.getChildren().remove(progressBtn);
                break;

            case STUDENT:
                leftSide.getChildren().remove(teacherButton);
                if (this.sisu.getActiveUser().getDegreeId() == null || this.sisu.getActiveUser().getDegreeId().isEmpty()) {
                    coursesButton.setDisable(true);
                    progressBtn.setDisable(true);
                }

                break;

            default:
                break;
        }
    }

    /**
     * Shows the sisu UI
     * @param stage UI element
     * @throws IOException 
     */
    public void show(Stage stage) throws IOException {
        FXMLLoader loader = new FXMLLoader(getClass().getResource("SisuUI.fxml"));
        loader.setController(this);
        root = loader.load();
        Scene scene = new Scene(root);
        stage.setTitle("Sisu 2.0");
        stage.setScene(scene);
        stage.show();

    }
}
