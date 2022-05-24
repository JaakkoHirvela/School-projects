package fi.tuni.prog3.sisu;

import org.json.JSONObject;
import org.jsoup.Jsoup;

/**
 * Class for holding an individual course's info.
 * @author markus
 */
public class DegreeCourse {

    private String id;
    private String name;
    private boolean isSelected = false;
    private String points;
    private String description;
    private String outcomes;
    private String grade = Grade.UKNOWN.getCode();

    public enum Grade {
        ZERO("0", "0"),
        ONE("1", "1"),
        TWO("2", "2"),
        TREE("3", "3"),
        FOUR("4", "4"),
        FIVE("5", "5"),
        UKNOWN("6", "Ei arvioitu");

        private String code;
        private String text;

        private Grade(String code, String text) {
            this.code = code;
            this.text = text;
        }

        public String getCode() {
            return code;
        }

        public String getText() {
            return text;
        }

        public static Grade getByCode(String gradeCode) {
            for (Grade g : Grade.values()) {
                if (g.code.equals(gradeCode)) {
                    return g;
                }
            }
            return null;
        }

        @Override
        public String toString() {
            return this.text;
        }
    }

    /**
     * Returns course id
     * @return course id
     */
    public String getId() {
        return id;
    }

    /**
     * Sets course id
     * @param id course id
     */
    public void setId(String id) {
        this.id = id;
    }

    /**
     * Returns course name
     * @return course name
     */
    public String getName() {
        return name;
    }

    /**
     * Sets course name
     * @param name course name
     */
    public void setName(String name) {
        this.name = name;
    }

    /**
     * Returns if course is selected in the course view
     * @return if course is selected
     */
    public boolean isSelected() {
        return isSelected;
    }

    /**
     * Set state if course is selected in the course view
     * @param isSelected true or false depending on what you want to set it on
     */
    public void setSelected(boolean isSelected) {
        this.isSelected = isSelected;
    }

     /**
     * Returns course credits
     * @return course credits
     */
    public String getPoints() {
        return points;
    }
    
    /**
     * Sets course credits
     * @param points course credits
     */
    public void setPoints(String points) {
        this.points = points;
    }

    /**
     * Sets course description
     * @param points course description
     */
    public void setDescription(String description) {
        this.description = description;
    }


    /**
     * Returns course description parsed from html
     * @return course description
     */
    public String getDescription(boolean plainText) {
        if (plainText) {
            return "Kuvaus:\n" + parseHtml(description);
        } else {
            return this.description;
        }

    }

    /**
     * Sets course outcomes
     * @param points course outcomes
     */
    public void setOutcomes(String outcomes) {
        this.outcomes = outcomes;
    }

    /**
     * Returns course outcomes parsed from html
     * @return course outcomes
     */
    public String getOutcomes(boolean plainText) {
        if (plainText) {
            return "Tavoitteet:\n" + parseHtml(outcomes);
        } else {
            return this.outcomes;
        }

    }

    /**
     * Parses html text to plain text
     * @param text text to be parsed
     * @return parsed text
     */
    public static String parseHtml(String text) {
        text = Jsoup.parse(text).toString();
        text = text.replace("</p>", "");
        text = text.replace("<p>", "");
        text = text.replace("</br>", "");
        text = text.replace("<br>", "");
        text = text.replace("</li>", "");
        text = text.replace("<li>", "");
        text = text.replace("</html>", "");
        text = text.replace("<html>", "");
        text = text.replace("</head>", "");
        text = text.replace("<head>", "");
        text = text.replace("</body>", "");
        text = text.replace("<body>", "");
        text = text.replace("</h5>", ":");
        text = text.replace("<h5>", "");
        text = text.replace("</h6>", ":\n");
        text = text.replace("<h6>", "");
        text = text.replace("</ul>", "");
        text = text.replace("<ul>", "");
        text = text.replace("\n \n \n", "\n");
        text = text.replace("\n \n", "");
        text = text.replace("\n  \n", "\n");
        return text;
    }

    /**
     * Converts course to json object
     * @return Course as json object
     */
    public JSONObject toJsonObject() {
        JSONObject course = new JSONObject();
        course.put("name", name);
        course.put("description", description);
        course.put("credits", points);
        course.put("outcomes", outcomes);
        course.put("isSelected", isSelected);
        course.put("grade", grade);
        return course;
    }

    /**
     * Converts json object to DegreeCourse object
     * @param course course as json object
     */
    public void readFromJson(JSONObject course) {
        this.name = course.getString("name");
        this.description = course.getString("description");
        this.points = course.getString("credits");
        this.outcomes = course.getString("outcomes");
        this.isSelected = course.getBoolean("isSelected");
        this.grade = course.getString("grade");
    }

    /**
     * Returns course grade
     * @return course grade
     */
    public String getGrade() {
        return this.grade;
    }

    /**
     * Sets course grade
     * @param newGrade course grade
     */
    public void setGrade(String newGrade) {
        this.grade = newGrade;
    }

}
