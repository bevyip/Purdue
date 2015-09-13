import java.util.Scanner;

/**
 * CS180 - Lab 03 - PersonTest.java
 *
 * (A brief description of the test case.
 * For example: A JUnit test case class to test 
 * the methodality of the Person class.)
 *
 * @author (Shreyaansh Bassi) <(sbassi@purdue.edu)>
 *
 * @lab (LAB 03)
 *
 * @date (09/17/14)
 */

/**
 * A JUnit test case class.
 * Every method starting with the word "test" will be called when running
 * the test with JUnit.
 */

public class Persons{
  String name;
  int hdl;
  int ldl;
  int other;
  int age;

  
  public Persons(String name, int hdl, int ldl, int other, int age){
    this.name = name;
    this.hdl = hdl;
    this.ldl = ldl;
    this.other = other;
    this.age = age;
  }
  
  public int getTotalCholesterol(){
    int tc = this.hdl + this.ldl + this.other;
    return tc;
  }
  
  public boolean isAdult(){
    if (this.age >= 18){
      return true;
    }
    else {
      return false;
    }
  }
  
  public boolean hasGoodTotalCholesterol(){
    if (isAdult()){
      if (getTotalCholesterol() < 200){
        return true;
      }
      else {
        return false;
      }
    }
    
    else {
      if (getTotalCholesterol() < 170){
        return true;
      }
      else {
        return false;
      }
    }
  }
  
  public boolean hasGoodHDL() {
    if (this.hdl >= 40){
      return true;
    }
    else {
      return false;
    }
  }
  
  public boolean hasGoodLDL(){
    if (isAdult()) {
      if (this.ldl <= 130) {
        return true;
      }
      else {
        return false;
      }
    }
    
    else {
      if (this.ldl <= 110) {
        return true;
      }
      else {
        return false;
      }
    }
  }
  
  public void printReport() {
    System.out.println(this.name + "'s Report");
    System.out.println("Age: " + this.age + " (" + (isAdult() ? "Adult" : "Child") + ")");
    System.out.println("Total Cholesterol: " + getTotalCholesterol() + 
                       " ("+(hasGoodTotalCholesterol()?"Good":"Bad")+")");
    System.out.println("LDL: " + this.ldl + " (" + (hasGoodLDL() ? "Good" : "Bad") + ")");
    System.out.println("HDL: " + this.hdl + " (" + (hasGoodHDL() ? "Good" : "Bad") + ")");
  }
  
  public static void main(String[] args) {
    String personname;
    int age;
    int ldl;
    int hdl;
    int other;
    Scanner in = new Scanner(System.in);
    System.out.println("Enter Person name:");
    personname = in.nextLine();
    System.out.println("Enter age:");
    age = in.nextInt();
    System.out.println("Enter LDL Cholesterol Level:");
    ldl = in.nextInt();
    System.out.println("Enter HDL Cholesterol Level:");
    hdl = in.nextInt();
    System.out.println("Enter Other Cholesterol Level:");
    other = in.nextInt();
    Persons p = new Persons(personname, hdl, ldl, other, age);
    p.printReport();
  }
}