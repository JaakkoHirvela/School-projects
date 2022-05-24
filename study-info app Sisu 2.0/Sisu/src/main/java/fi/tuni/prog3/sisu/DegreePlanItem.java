package fi.tuni.prog3.sisu;

import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONObject;

import javafx.util.Pair;

/**
 * Class for holding an individual module's info. (study module, degree programme etc...)
 * @author markus
 */
public class DegreePlanItem {

    private String id;
    private String name;
    private String description;
    private int points;
    private ArrayList<DegreePlanItem> degreePlans = new ArrayList<>();
    private ArrayList<DegreeCourse> degreeCourses = new ArrayList<>();

    @Override
    public String toString(){
      return name;
    }
    
    /**
     * Returns module id
     * @return module id
     */
    public String getId() {
        return id;
    }

    /**
     * Changes module id
     * @param id
     */
    public void setId(String id) {
        this.id = id;
    }

    /**
     * Returns module name
     * @return module name
     */
    public String getName() {
        return name;
    }

    /**
     * Changes module name
     * @param name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Sets description for the module
     * @param description
     */
    public void setDescription(String description) {
        description = DegreeCourse.parseHtml(description);
        description = "Kuvaus:\n" + description;
        this.description = description;
    }

    /**
     * Returns module description
     * @return module description
     */
    public String getDescription() {
        return description;
    }

    /**
     * Returns module credits
     * @return module credits
     */
    public int getPoints() {
        return points;
    }

    /**
     * Sets module points
     * @param points
     */
    public void setPoints(int points) {
        this.points = points;
    }

    /**
     * Returns modules under itself
     * @return modules under itself
     */
    public ArrayList<DegreePlanItem> getDegreePlans() {
        return degreePlans;
    }

    /**
     * Sets modules under itself 
     * @param degreePlans new submodules
     */
    public void setDegreePlans(ArrayList<DegreePlanItem> degreePlans) {
        this.degreePlans = degreePlans;
    }

    /**
     * Adds one submodule
     * @param degreePlan submodule
     */
    public void setDegreePlan(DegreePlanItem degreePlan) {
        this.degreePlans.add(degreePlan);
    }

    /**
     * Returns courses under the module
     * @return courses under the module
     */
    public ArrayList<DegreeCourse> getDegreeCourses() {
        return degreeCourses;
    }

    /**
     * Sets courses under the module
     * @param degreeCourses
     */
    public void setDegreeCourses(ArrayList<DegreeCourse> degreeCourses) {
        this.degreeCourses = degreeCourses;
    }

    /**
     * Adds course to list of courses under the module
     * @param degreeCourse
     */
    public void setDegreeCourse(DegreeCourse degreeCourse) {
        this.degreeCourses.add(degreeCourse);
    }

    /**
     * Sets all submodules from Strings to DegreePlanItems or DegreeCourses
     * @param children module ids and types
     */
    public void setSubModules(ArrayList<Pair<String, String>> children){
        for (Pair<String, String> child : children) { // key = groupId, value = rule/type
            addChild(child);
        }
    }

    /**
     * Adds submodules to right lists
     * @param child submodule id and type
     */
    public void addChild(Pair<String, String> child) {
        if (child.getValue().contains("ModuleRule")) {
          DegreePlanItem subModule = DataManager.getModule(child.getKey());
          // Prevent duplicates:
          if (degreePlans.stream().filter(DegreePlanItem -> subModule.getId().equals(DegreePlanItem.getId())).findFirst().orElse(null) == null){
            setDegreePlan(subModule);
          }
    
        } else if (child.getValue().contains("CourseUnitRule")) {
             DegreeCourse course = DataManager.getCourse(child.getKey());
            // Prevent duplicates:
            if (degreeCourses.stream().filter(DegreeCourse -> course.getId().equals(DegreeCourse.getId())).findFirst().orElse(null) == null){
                setDegreeCourse(course);
            }
        }
    }

    /**
     * Converts module to json object
     * @return module as json object
     */
    public JSONObject toJsonObject(){
        JSONObject module = new JSONObject();
        module.put("name", name);
        
        if(description == null){
            module.put("description", description);
        }
        
        module.put("credits", points + "");
        
        JSONArray courses = new JSONArray();
        for(DegreeCourse course : degreeCourses){
            courses.put(course.toJsonObject());
        }
        module.put("courses", courses);
        
        JSONArray modules = new JSONArray();
        for(DegreePlanItem submodule : degreePlans){
            modules.put(submodule.toJsonObject());
        }
        module.put("submodules", modules);
        return module;
    }

    /**
     * Reads module from json file and creates module from it
     * @param module
     */
    public void readFromJson(JSONObject module){
        this.name = module.getString("name");
        
        if(module.has("description")){
           this.description = module.getString("description"); 
        }
        
        this.points = Integer.parseInt(module.getString("credits"));
        
        JSONArray courses = module.getJSONArray("courses");
        for(int i = 0; i < courses.length(); ++i){
            JSONObject jsonCourse = courses.getJSONObject(i);
            DegreeCourse course =  new DegreeCourse();
            course.readFromJson(jsonCourse);
            degreeCourses.add(course);
        }
        
        JSONArray modules = module.getJSONArray("submodules");
        for(int i = 0; i < modules.length(); ++i){
            JSONObject jsonModules = modules.getJSONObject(i);
            DegreePlanItem submodule =  new DegreePlanItem();
            submodule.readFromJson(jsonModules);
            degreePlans.add(submodule);
        }   
    }
}
