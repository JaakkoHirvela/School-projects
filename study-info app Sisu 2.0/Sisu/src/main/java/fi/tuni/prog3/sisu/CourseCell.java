package fi.tuni.prog3.sisu;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.CheckBox;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.Tooltip;

/**
 *
 * Class for displaying individual courses in course view.
 */
public class CourseCell extends ListCell<DegreeCourse> {

    @FXML
    private ResourceBundle resources;

    @FXML
    private URL location;

    @FXML
    private Label courseName;

    @FXML
    private CheckBox courseSelection;

    @FXML
    void makeSelection(ActionEvent event) {
        this.getItem().setSelected(courseSelection.isSelected());
    }

  /**
   * Default constructor.
   */
  public CourseCell() {
        loadFXML();
    }

    private void loadFXML() {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("CourseCell.fxml"));
            loader.setController(this);
            loader.setRoot(this);
            loader.load();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

  /**
   * Updates the course cell.
   * @param item Course cell to be updated.
   * @param empty Tells whether cell has an actual course (true) or not (false).
   */
  @Override
    protected void updateItem(DegreeCourse item, boolean empty) {
        super.updateItem(item, empty);
        if (empty || item == null) {
            setText(null);
            setContentDisplay(ContentDisplay.TEXT_ONLY);

        } else {
            courseName.setText(item.getName());
            courseSelection.setSelected(item.isSelected());
            setContentDisplay(ContentDisplay.GRAPHIC_ONLY);
            setTooltip(new Tooltip("Klikkaa saadaksesi lisätietoja"));
        }
    }

}
