package fi.tuni.prog3.sisu;

import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.ArrayList;
import javafx.util.Pair;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Fetches study-info using Sisu API.
 * 
 * @author Samuli
*/
public class DataManager {
  
    private static ArrayList<DegreePlanItem> degrees;
    private static ArrayList<Pair<String, String>> modules; // Pair< groupId, type >
    private static DegreeCourse course;
    private static DegreePlanItem module;

  /**
   * Returns a list of all the degree programmes in Sisu.
   * @return a list of all the degree programmes.
   */
  public static ArrayList<DegreePlanItem> getDegrees(){
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create("https://sis-tuni.funidata.fi/kori/api/module-search?curriculumPeriodId=uta-lvv-2021&universityId=tuni-university-root-id&moduleType=DegreeProgramme&limit=1000")).build();
        client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
              .thenApply(HttpResponse::body)
              .thenAccept(DataManager::readDegrees)
              .join();
        return degrees;
    }

  /**
   * Reads all degree programmes into a private list. Works alongside getDegrees().
   * @param responseBody response received with HttpRequest in getDegrees().
   */
  public static void readDegrees(String responseBody){
        ArrayList<DegreePlanItem> dgrs = new ArrayList<>();
        JSONObject all = new JSONObject(responseBody);
        JSONArray info = all.getJSONArray("searchResults");
        for (int i = 0; i < info.length(); ++i){
            JSONObject degree = info.getJSONObject(i);
            String name = degree.getString("name");
            String id = degree.getString("id");
            Integer credits;
            String description;
            try{
              credits = (degree.getJSONObject("targetCredits").getInt("min"));  
            } catch(JSONException e){
              credits = 0;
            }
            try{
              description = degree.getJSONObject("contentDescription").getString("fi");
            } catch(JSONException e){
              description = "";
            }
            // Do not load submodules here (performance):
            DegreePlanItem degreeItem = new DegreePlanItem();
            degreeItem.setDescription(description);
            degreeItem.setId(id);
            degreeItem.setName(name);
            degreeItem.setPoints(credits);
            dgrs.add(degreeItem);
            //System.out.println(name + "  " + id);
        }
        degrees = dgrs;
    }

  /**
   * Returns all submodules under module with given id.
   * @param id id of the parent module.
   * @return all submodules under module with given id.
   */
  public static ArrayList<Pair<String, String>> getSubModules(String id){
        String url = "https://sis-tuni.funidata.fi/kori/api/modules/" + id;
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create(url)).build();
        client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
              .thenApply(HttpResponse::body)
              .thenAccept(DataManager::readSubModules)
              .join();
        return modules;
    }
    
  /**
   * Reads all submodules into a private list. Works alongside getSubmodules(). 
   * @param responseBody response String received from Sisu with HttpRequest in getSubmodules().
   */
  public static void readSubModules(String responseBody){
        ArrayList<Pair<String, String>> module_ids = new ArrayList<>();
        JSONObject degreeProgram = new JSONObject(responseBody);
        JSONObject compositeRule = degreeProgram.getJSONObject("rule");
        ArrayList<JSONObject> compositeRules = new ArrayList<>();
        
        while(true){
          if(compositeRule.has("rule")){
            compositeRule = compositeRule.getJSONObject("rule");
          } else if(compositeRule.has("rules")){
            JSONArray rules = compositeRule.getJSONArray("rules");
            Boolean brk = true;
            for(int i = 0; i < rules.length(); ++i){
              JSONObject rule = rules.getJSONObject(i);
              if(rule.getString("type").equals("CompositeRule")){
                compositeRule = rule;
                if(rules.length() != 1){compositeRules.add(rule);}
                brk = false;
              } else if (rule.has("moduleGroupId")){
                module_ids.add(new Pair<String,String>(rule.getString("moduleGroupId"), rule.getString("type")));
              } else if (rule.has("courseUnitGroupId")){
                module_ids.add(new Pair<String,String>(rule.getString("courseUnitGroupId"), rule.getString("type")));
              }
            }
            if(brk){break;}
          }
        } 

        if(compositeRules.size() != 0){
          for(JSONObject comprule : compositeRules){
            JSONArray inner_rules = comprule.getJSONArray("rules");
            for(int i = 0; i < inner_rules.length(); ++i){
              JSONObject rule = inner_rules.getJSONObject(i);
              if (rule.has("moduleGroupId")){
                module_ids.add(new Pair<String,String>(rule.getString("moduleGroupId"), rule.getString("type")));
              } if (rule.has("courseUnitGroupId")){
                module_ids.add(new Pair<String,String>(rule.getString("courseUnitGroupId"), rule.getString("type")));
              }
            }    
          }
        }

        modules = module_ids;
    } 
    
  /**
   * Returns a DegreePlanItem with a specific id.
   * @param id id of the object to be fetched.
   * @return a DegreePlanItem with given id.
   */
  public static DegreePlanItem getModule(String id){
      String url;
      if(id.contains("otm")){
        url = "https://sis-tuni.funidata.fi/kori/api/modules/" + id;
      } else {
        url = "https://sis-tuni.funidata.fi/kori/api/modules/by-group-id?groupId=" + id + "&universityId=tuni-university-root-id";
      }
      HttpClient client = HttpClient.newHttpClient();
      HttpRequest request = HttpRequest.newBuilder().uri(URI.create(url)).build();
      client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
            .thenApply(HttpResponse::body)
            .thenAccept(DataManager::readModule)
            .join();
      return module;
    }
    
  /**
   * Reads a single module from response received from Sisu API. Works alongside getModule().
   * @param responseBody response received from Sisu API with HttpRequest.
   */
  public static void readModule(String responseBody){
      try{
        JSONObject jsonModule = getJSONObject(responseBody);
        String name;
        Integer credits;
        String description;
        try{
          if (jsonModule.getJSONObject("name").has("fi")){
            name = jsonModule.getJSONObject("name").getString("fi");
          }
          else{
            name = jsonModule.getJSONObject("name").getString("en");
          }
        }
        catch(JSONException e){
          name = null;
        }
        try{
          credits =(jsonModule.getJSONObject("targetCredits").getInt("min"));
        }
        catch(JSONException e){
          credits = 0;
        }
        try{
          description = jsonModule.getJSONObject("contentDescription").getString("fi");
        }
        catch(JSONException e){
          description = "";
        }
        String id = jsonModule.getString("id"); 
        DegreePlanItem newModule = new DegreePlanItem();
        newModule.setId(id);
        newModule.setName(name);
        newModule.setPoints(credits);
        newModule.setDescription(description);
        newModule.setSubModules(getSubModules(id));
        module = newModule;
      }
      catch(JSONException e){
        System.out.println(e);
      }

    }

  /**
   * Returns a DegreeCourse with a specific id.
   * @param id id of the object to be fetched.
   * @return a DegreeCourse with given id.
   */
  public static DegreeCourse getCourse(String id){
        String url =  "https://sis-tuni.funidata.fi/kori/api/course-units/by-group-id?groupId="+ id +"&universityId=tuni-university-root-id";
        HttpClient client = HttpClient.newHttpClient();
        HttpRequest request = HttpRequest.newBuilder().uri(URI.create(url)).build();
        client.sendAsync(request, HttpResponse.BodyHandlers.ofString())
              .thenApply(HttpResponse::body)
              .thenAccept(DataManager::readCourse)
              .join();
        return course;
    }

  /**
   * Reads a single course from response received from Sisu API. Works alongside getCourse().
   * @param responseBody response received from Sisu API with HttpRequest.
   */
  public static void readCourse(String responseBody){
      try{
        JSONObject courseUnit = getJSONObject(responseBody);
        String id = courseUnit.getString("id");
        String name;
        if (courseUnit.getJSONObject("name").has("fi")){
          name = courseUnit.getJSONObject("name").getString("fi");
        }
        else{
          name = courseUnit.getJSONObject("name").getString("en");
        }
        Integer creditsMin = 0;
        Integer creditsMax = 0;
        if (courseUnit.has("credits")){
          creditsMin = courseUnit.getJSONObject("credits").getInt("min");
          try{
            creditsMax = courseUnit.getJSONObject("credits").getInt("max");
          }
          catch(JSONException e){
            creditsMax = courseUnit.getJSONObject("credits").getInt("min");
          }
        }
        String description;
        try{
          if(courseUnit.getJSONObject("content").has("fi")){
            description = courseUnit.getJSONObject("content").getString("fi");
          }
          else{
            description = courseUnit.getJSONObject("content").getString("en");
          }
        }
        catch(JSONException e){
          description = "";
        }
        String outcomes;
        try{
          if(courseUnit.getJSONObject("outcomes").has("fi")){
            outcomes = courseUnit.getJSONObject("outcomes").getString("fi");
          }
          else{
            outcomes = courseUnit.getJSONObject("outcomes").getString("en");
          }
        }
        catch(JSONException e){
          outcomes = "";
        }
        DegreeCourse dCourse = new DegreeCourse();
        dCourse.setDescription(description);
        dCourse.setId(id);
        dCourse.setName(name);
        dCourse.setOutcomes(outcomes);
        if(creditsMax == creditsMin){
          dCourse.setPoints(creditsMax +"");
        } else {
          dCourse.setPoints(creditsMin + "-" + creditsMax);
        }
        course = dCourse;
      }
      catch(JSONException e){
        System.out.println(e);
      }
    }
    
    private static JSONObject getJSONObject(String responseBody){
      JSONObject jsonObj;
      char firstChar = responseBody.charAt(0);
      if (firstChar == '['){
        JSONArray jsonArr = new JSONArray(responseBody);
        jsonObj = jsonArr.getJSONObject(0);
      }
      else{
        jsonObj = new JSONObject(responseBody);
      }
      return jsonObj;
    }
}
