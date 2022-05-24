package fi.tuni.prog3.sisu;

/**
 * Class to save user information
 */
public class User implements IUser {

    private String name = "";
    private String lastName = "";
    private int age;
    private String degreeId;

    /**
     * Constructor
     * @param name users name
     * @param lastName users lastname
     * @param age users age
     * @param degreeId users degree programmes id
     */
    public User(String name, String lastName, int age, String degreeId) {
        super();
        this.name = name;
        this.lastName = lastName;
        this.age = age;
        this.degreeId = degreeId;
    }

    /**
     * Constructor with only a name
     * @param name users name
     */
    User(String name) {
        this.name = name;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public void setName(String name) {
        this.name = name;
    }

    @Override
    public String getLastName() {
        return lastName;
    }

    @Override
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    @Override
    public int getAge() {
        return age;
    }

    @Override
    public void setAge(int age) {
        this.age = age;
    }

    /**
     * Returns degree programme id
     * @return degree programme id
     */
    public String getDegreeId() {
        return degreeId;
    }

    /**
     * Sets degree programme id
     * @param degreeId
     */
    public void setDegreeId(String degreeId) {
        this.degreeId = degreeId;
    }

    @Override
    public String toString() {
        return this.getName() + " " + this.getLastName();
    }

}
