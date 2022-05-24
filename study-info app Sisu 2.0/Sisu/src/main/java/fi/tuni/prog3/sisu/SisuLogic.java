package fi.tuni.prog3.sisu;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.prefs.BackingStoreException;
import java.util.prefs.InvalidPreferencesFormatException;
import java.util.stream.Collectors;

import javafx.util.Pair;
import org.json.JSONObject;

/**
 * This class handles all the business logic of the program.
 *
 * fireListeners() updates the views.
 * 
 * @author markus
 */
public class SisuLogic {

    private final ArrayList<SisuLogicListener> listeners = new ArrayList<>();

    private ArrayList<User> users = new ArrayList<>();

    public enum Mode {
        TEACHER,
        STUDENT,
        UKNOWN
    }
    private Mode mode = Mode.UKNOWN;

    private User user = null;
    private ArrayList<DegreePlanItem> degreePrograms = new ArrayList<>();
    private DegreePlanItem userDegreeProgramme;
    private DegreePlanItem root;

    /**
     * Constructor which loads the user info
     */
    public SisuLogic() {
        loadUsers();
    }

    /**
     * Returns user object if on the list
     * @param name name which is searched
     * @return the object of the user
     */
    public User getUserByName(String name) {
        User found = users.stream()
                .filter(customer -> name.equals(customer.getName()))
                .findAny()
                .orElse(null);
        return found;
    }

    /**
     * Returns active user
     * @return active user
     */
    public User getActiveUser() {
        return user;
    }

    /**
     * Changes the active user
     * @param user new active user
     */
    public void setActiveUser(User user) {
        this.user = user;
        this.fireListeners();
    }

    /**
     * Creates new user and sets it as the active user
     * @param name name of the new active user
     */
    public void createUser(String name) {
        User newUser = new User(name);
        users.add(newUser);
        user = newUser;
    }

    /**
     * Returns all users
     * @return all users
     */
    public ArrayList<User> getUsers() {
        return users;
    }

    /**
     * Loads users using Logger class 
     */
    private void loadUsers() {
        if (users.isEmpty()) {
            try {

                this.users.addAll(new UserPreferences().getUserPreferences());

            } catch (IOException | InvalidPreferencesFormatException ex) {
                Logger.getLogger(SisuLogic.class.getName()).log(Level.SEVERE, null, ex);
            }

            fireListeners();
        }

    }

    /**
     * Saves user preferences
     * @throws IOException if writing to the specified output stream results in an IOException.
     * @throws BackingStoreException
     * @throws InvalidPreferencesFormatException
     */
    public void saveUser() throws IOException, BackingStoreException, InvalidPreferencesFormatException {
        removeSelection();

        UserPreferences.saveUserPreferences(user);
        fireListeners();
    }

    /**
     * Returns list of all degrees in sisu
     * @return list of all degrees
     */
    public ArrayList<DegreePlanItem> fecthDegrees() {
        if (degreePrograms.isEmpty()) {
            this.degreePrograms = DataManager.getDegrees();
        }
        return this.degreePrograms;
    }

    /**
     * Returns users degree programme
     * @param user
     * @return degree programme
     */
    public DegreePlanItem fetchDegreePlan(User user) {

        try {
            root = fetchDegreePlanFromFile(user);
            return root;

        } catch (Exception e) {
            fecthDegrees();
            DegreePlanItem plan = new DegreePlanItem();
            userDegreeProgramme = degreePrograms.get(degreePrograms.stream()
                    .map(degreeProgram -> degreeProgram.getId())
                    .collect(Collectors.toList())
                    .indexOf(user.getDegreeId()));
            plan.setName(userDegreeProgramme.getName());
            plan.setPoints(userDegreeProgramme.getPoints());

            ArrayList<Pair<String, String>> submodules = DataManager.getSubModules(userDegreeProgramme.getId());
            for (Pair<String, String> module : submodules) {
                DegreePlanItem mod = DataManager.getModule(module.getKey());
                plan.setDegreePlan(mod);
            }
            root = plan;
            return plan;
        }
    }

    /**
     * If there is a savestate of user this method reads it from the json file and sets it as the active degree plan
     * @param activeUser
     * @return degree programme
     * @throws IOException if no savestate is found
     */
    public DegreePlanItem fetchDegreePlanFromFile(User activeUser) throws IOException {
        String filename = user.getName() + user.getLastName() + user.getDegreeId() + ".json";
        File jsonFile = new File(filename);
        String content = new String(Files.readAllBytes(Paths.get(jsonFile.toURI())));
        JSONObject rootObj = new JSONObject(content);
        DegreePlanItem rootItem = new DegreePlanItem();
        rootItem.readFromJson(rootObj);
        root = rootItem;
        return root;
    }

    /**
     * Saves the degree programme and its structure on a json file 
     * @throws FileNotFoundException Should never happen, beacuse it writes the file.
     */
    public void saveSelection() throws FileNotFoundException {
        JSONObject json = root.toJsonObject();
        String filename = user.getName() + user.getLastName() + user.getDegreeId() + ".json";

        File jsonFile = new File(filename);
        try ( PrintWriter writer = new PrintWriter(jsonFile)) {
            writer.println(json.toString(2));
        }
        fireListeners();
    }

    /**
     * Deletes the active users save file.
     * @throws FileNotFoundException if no save file is found
     */
    public void removeSelection() throws FileNotFoundException {
        String filename = user.getName() + user.getLastName() + user.getDegreeId() + ".json";
        File jsonFile = new File(filename);
        if (jsonFile.exists()) {
            jsonFile.delete();
        }
        this.root = null;
        this.userDegreeProgramme = null;
    }
    
    /**
     * Returns the active degree programme
     * @return the active degree programme
     */
    public DegreePlanItem getRoot() {
        return root;
    }

    /**
     * Listens for requests to change view
     */
    private void fireListeners() {
        for (SisuLogicListener listener : listeners) {
            listener.updateView();
        }
    }

    /**
     * Register to listen changes in SisuLogic class
     *
     * @param listener
     */
    public void addListener(SisuLogicListener listener) {
        this.listeners.add(listener);
    }

    /**
     * Changes the active mode
     * @param mode the new mode
     */
    void selectMode(Mode mode) {
        this.mode = mode;
    }

    /**
     * Returns the active mode
     * @return the active mode
     */
    public Mode getMode() {
        return mode;
    }

}
