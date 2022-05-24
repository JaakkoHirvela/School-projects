package fi.tuni.prog3.sisu;

import java.net.URL;
import java.util.ArrayList;
import java.util.ResourceBundle;
import javafx.application.Platform;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;

/**
 * Class to control UI for user preferences
 */
public class UserInfoController extends BaseController implements Initializable {

    @FXML
    private TextField age;

    @FXML
    private ComboBox<DegreePlanItem> degreeSelector;

    @FXML
    private TextField profileName;

    @FXML
    private TextField sureName;

    @FXML
    private Label errorMessage;

    public UserInfoController(String name, String fxml, SisuLogic sisu) {
        super(name, fxml, sisu);
    }

    @FXML
    /**
     * Saves user info on file
     * @param event Click of the save button
     */
    void saveUserInfo(ActionEvent event) {
        try {
            errorMessage.setText("");
            if (profileName.getText().isBlank()) {
                errorMessage.setText("Etunimi on pakollinen");
                return;
            }

            if (sureName.getText().isBlank()) {
                errorMessage.setText("Sukunimi on pakollinen");
                return;
            }

            if (age.getText().isBlank()) {
                errorMessage.setText("Ikä on pakollinen");
                return;
            }

            if (this.degreeSelector.getValue() == null) {
                errorMessage.setText("Tutkinto ohjelma on pakollinen");
                return;
            }
            User user = getSisu().getActiveUser();
            user.setName(profileName.getText());
            user.setLastName(sureName.getText());
            user.setAge(Integer.parseInt(age.getText()));
            user.setDegreeId(this.degreeSelector.getValue().getId());

            getSisu().saveUser();
        } catch (Exception e) {
            errorMessage.setText("Tarkista että annetut tiedot ovat oikein");
        }

    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        User user = null;
        user = getSisu().getActiveUser();

        if (user == null) {
            return;
        }

        this.age.setText(String.valueOf(user.getAge()));
        this.profileName.setText(user.getName());
        this.sureName.setText(user.getLastName());
        String degreeId = user.getDegreeId();
        degreeSelector.setCellFactory((ListView<DegreePlanItem> p) -> {
            final ListCell<DegreePlanItem> cell = new ListCell<DegreePlanItem>() {

                @Override
                protected void updateItem(DegreePlanItem t, boolean bln) {
                    super.updateItem(t, bln);

                    if (t != null) {
                        setText(t.getName());
                    } else {
                        setText(null);
                    }
                }

            };

            return cell;
        });

        Thread one = new Thread() {
            @Override
            public void run() {
                ArrayList<DegreePlanItem> items = getSisu().fecthDegrees();
                Platform.runLater(() -> {
                    ObservableList<DegreePlanItem> siteList
                            = FXCollections.observableArrayList(
                                    items
                            );
                    degreeSelector.setItems(siteList);
                    if (degreeId != null && !degreeId.isEmpty()) {
                        DegreePlanItem found = siteList.stream()
                                .filter(x -> x.getId().equals(degreeId))
                                .findAny()
                                .orElse(null);
                        degreeSelector.setValue(found);
                    }
                });

            }
        };
        one.start();

    }

}
