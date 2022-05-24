package fi.tuni.prog3.sisu;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.prefs.BackingStoreException;
import java.util.prefs.InvalidPreferencesFormatException;
import java.util.prefs.Preferences;

/**
 * Class for saving user-information.
 * 
 * @author Jaakko
 * 
 */
public class UserPreferences {

    private final static Preferences userPrefs = Preferences.userNodeForPackage(fi.tuni.prog3.sisu.UserPreferences.class);
    final static String NAME = "name";
    final static String LAST_NAME = "lastName";
    final static String AGE = "age";
    final static String DEGREE_ID = "degreeId";

  /**
   * Default constructor.
   */
  public UserPreferences() {

    }

  /**
   * Saves user's information into a file named: "user[first_name][last_name].xml".
   * @param user User's information to be saved
   * @throws IOException if writing to the specified output stream results in an IOException.
   * @throws BackingStoreException if preference data cannot be read from backing store.
   */
  public static void saveUserPreferences(User user) throws IOException, BackingStoreException {
        String fileName = "user" + user.getName()+user.getLastName()+ ".xml";
        File file = new File(fileName);
        if(file.exists()){
            file.delete();
        }
        userPrefs.put(NAME, user.getName());
        userPrefs.put(LAST_NAME, user.getLastName());
        userPrefs.putInt(AGE, user.getAge());
        userPrefs.put(DEGREE_ID, user.getDegreeId());

        userPrefs.exportNode(new FileOutputStream(fileName));
    }

  /**
   * Returns a list of all saved user informations.
   * @return A list of all saved user informations.
   * @throws IOException if reading from the specified input stream results in an IOException.
   * @throws InvalidPreferencesFormatException Data on input stream does not constitute a valid XML document with the mandated document type.
   */
  public ArrayList<User> getUserPreferences() throws IOException, InvalidPreferencesFormatException {
        ArrayList<User> users = new ArrayList<>();
        File folder = new File(".");
        File[] listOfFiles = folder.listFiles();

        for (File file : listOfFiles) {
            if (file.isFile() && file.getName().startsWith("user")) {
                System.out.println(file.getName());

                try {
                    InputStream is = new BufferedInputStream(new FileInputStream(file.getName()));
                    Preferences.importPreferences(is);
                } catch (IOException e) {
                    System.out.print(e);
                }
                users.add( new User(userPrefs.get(NAME, "null"), userPrefs.get(LAST_NAME, ""),
                        userPrefs.getInt(AGE, 0), userPrefs.get(DEGREE_ID, "")));
            }
        }
        return users ;
    }

}
