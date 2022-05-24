/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Interface.java to edit this template
 */
package fi.tuni.prog3.sisu;

/**
 * Interface class for different types of users. (teacher, student).
 * @author markus
 */
public interface IUser {

  /**
   * Return age of the user.
   * @return age of the user.
   */
  int getAge();

  /**
   * Return last name of the user.
   * @return last name of the user.
   */
  String getLastName();

  /**
   * Return first name of the user.
   * @return first name of the user.
   */
  String getName();

  /**
   * Set age of the user.
   * @param age age of the user.
   */
  void setAge(int age);

  /**
   * Set last name of the user.
   * @param lastName last name of the user.
   */
  void setLastName(String lastName);

  /**
   * Set first name of the user.
   * @param name first name of the user.
   */
  void setName(String name);
    
}
