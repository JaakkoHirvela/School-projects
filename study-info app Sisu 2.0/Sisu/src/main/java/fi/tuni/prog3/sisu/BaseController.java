package fi.tuni.prog3.sisu;

/**
 * Base class for UI controllers.
 */
public class BaseController {

    private String name;
    private String fxml;
    private SisuLogic sisu;

  /**
   * Default constructor for BaseController
   * @param name Name of the controller.
   * @param fxml Name of the corresponding .fxml file.
   * @param sisu The singleton SisuLogic object.
   */
  public BaseController(String name, String fxml, SisuLogic sisu) {
        super();
        this.name = name;
        this.fxml = fxml;
        this.sisu = sisu;
    }

  /**
   * Returns the name of the controller
   * @return Name of the controller.
   */
  public String getName() {
        return name;
    }

  /**
   * Returns the name of the corresponding .fxml file.
   * @return Name of the corresponding .fxml file.
   */
  public String getFxml() {
        return fxml;
    }

  /**
   * Returns the singleton SisuLogic object.
   * @return Singleton SisuLogic object.
   */
  protected SisuLogic getSisu() {
        return sisu;
    }
}
