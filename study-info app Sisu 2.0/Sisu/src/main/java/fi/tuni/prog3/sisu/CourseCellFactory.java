
package fi.tuni.prog3.sisu;

import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.util.Callback;

/**
 * Factory class that builds new CourseCell objects when called.
 * @author markus
 */
    public class CourseCellFactory implements Callback<ListView<DegreeCourse>, ListCell<DegreeCourse>> {

  /**
   * Returns new CourseCell object.
   * @param param 
   * @return New CourseCell object
   */
  @Override
        public ListCell<DegreeCourse> call(ListView<DegreeCourse> param) {
            return new CourseCell();
        }
    }
