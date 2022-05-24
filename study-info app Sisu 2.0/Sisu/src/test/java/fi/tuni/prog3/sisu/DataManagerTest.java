package fi.tuni.prog3.sisu;


import java.util.ArrayList;
import javafx.util.Pair;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;

/**
 * Test class for DataManager
 * @author Jaakko
 */
public class DataManagerTest {
  
  public DataManagerTest() {
  }
  
  @BeforeAll
  public static void setUpClass() {
  }
  
  @AfterAll
  public static void tearDownClass() {
  }
  
  @BeforeEach
  public void setUp() {
  }
  
  @AfterEach
  public void tearDown() {
  }
  /**
   * Test of getCourse method, of class DataManager.
   */
  @Test
  public void testGetCourse() {
    System.out.println("getCourse");
    String id = "tut-cu-g-45460";
    String expResult = "Signaalit ja mittaaminen";
    String result = DataManager.getCourse(id).getName();
    assertEquals(expResult, result);
  }

  /**
   * Test of getDegrees method, of class DataManager.
   */
  @Test
  public void testGetDegrees() {
    System.out.println("getDegrees");
    String expResult = "Anestesiologian ja tehohoidon erikoislääkärikoulutus (55/2020)";
    ArrayList<DegreePlanItem> degrees = DataManager.getDegrees();
    DegreePlanItem thirdItem = degrees.get(2);
    assertEquals(expResult, thirdItem.getName());
  }

  /**
   * Test of getSubModules method, of class DataManager.
   */
  @Test
  public void testGetSubModules() {
    System.out.println("getSubModules");
    String id = "otm-af70be28-9bf5-49f7-b8fc-41a2bafbf2f2";
    String expResult = "tut-cu-g-45510"; // second submodule
    ArrayList<Pair<String, String>> subModules = DataManager.getSubModules(id);
    assertEquals(expResult, subModules.get(1).getKey());
  }

  /**
   * Test of getModule method, of class DataManager.
   */
  @Test
  public void testGetModule() {
    System.out.println("getModule");
    String id = "otm-d713f234-c3cd-4d12-aafe-c28c8da0240b";
    String expResult = "Doctoral Programme in Dynamic Wearable Applications with Privacy Constraints";
    DegreePlanItem degree = DataManager.getModule(id);
    assertEquals(expResult, degree.getName());
  }

}
