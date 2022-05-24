package fi.tuni.prog3.sisu;

import fi.tuni.prog3.sisu.SisuLogic.Mode;
import java.io.IOException;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.stage.Modality;
import javafx.stage.Stage;

/**
 * Controller class handling the login view window at program start.
 */
public class LoginViewController extends BaseController {

    private Stage stage;

  /**
   * Default constructor for BaseController
   * @param name Name of the controller.
   * @param fxml Name of the corresponding .fxml file.
   * @param sisu The singleton SisuLogic object.
   */
  public LoginViewController(String name, String fxml, SisuLogic sisu) {
        super(name, fxml, sisu);
        loadFXML();
    }

    @FXML
    private Button studentBtn;

    @FXML
    private Button teacherBtn;

    @FXML
    private TextField userName;

    @FXML
    private Label messageLbl;

    @FXML
    void studentAction(ActionEvent event) {
        messageLbl.setText("");
        if (userName.getText().isEmpty()) {
            messageLbl.setText("Nimi on pakollinen");
            return;
        }
        User user = getSisu().getUserByName(userName.getText());
        if (user == null) {
            getSisu().createUser(userName.getText());
        } else {
            getSisu().setActiveUser(user);
        }

        getSisu().selectMode(Mode.STUDENT);
        stage.close();
    }

    @FXML
    void teacherAction(ActionEvent event) {
        getSisu().selectMode(Mode.TEACHER);
        stage.close();

    }

    private void loadFXML() {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource(getFxml()));
            loader.setController(this);
            Parent parent = loader.load();
            stage = new Stage();
            stage.setScene(new Scene(parent));
            stage.setResizable(false);
            stage.initModality(Modality.APPLICATION_MODAL);
            stage.alwaysOnTopProperty();

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    void show() {
        stage.showAndWait();
    }

}
