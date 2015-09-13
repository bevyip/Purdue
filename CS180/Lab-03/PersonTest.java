import junit.framework.TestCase;

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
public class PersonTest extends TestCase {
  private Persons adult_badTC;
  private Persons child_goodHDL;
  private Persons adult_badLDL;
  private Persons adult_goodTC;  
  private Persons child_goodLDL;
  private Persons adult_badHDL;
  private Persons adult_goodHDL;
  private Persons adult_goodLDL;
  
  protected void setUp() throws Exception {
    super.setUp();
    // initialize test people
    //Example
    adult_badTC = new Persons("0022212345",100,100,100,19);
    adult_goodTC = new Persons("0027234234",90,90,10,19);
    adult_badLDL = new Persons("0022212423",90,150,10,19);
    adult_goodLDL = new Persons("0022212345",90,90,40,19);
    adult_badHDL = new Persons("00222123466",30,110,10,19);
    adult_goodHDL = new Persons("0022212390",50,110,20,19);
    adult_badLDL = new Persons("0022212347",100,140,40,19);
    child_goodHDL = new Persons("0022212346",40,100,40,10);
    child_goodLDL = new Persons("0022212123",100,50,10,10);
    
  }
  
  /**
   * A test method.
   * (Replace "X" with a name describing the test.  You may write as
   * many "testSomething" methods in this class as you wish, and each
   * one will be called when running JUnit over this class.)
   */
  public void testGetTotalCholesterol() {
    assertEquals(300,adult_badTC.getTotalCholesterol());
    assertEquals(190,adult_goodTC.getTotalCholesterol());
  }
  public void testHasGoodTotalCholesterol() {
    assertEquals(true,adult_goodTC.hasGoodTotalCholesterol());
    assertEquals(false,adult_badTC.hasGoodTotalCholesterol());
  }
  public void testHasGoodLDL() {
    assertEquals(true,adult_goodLDL.hasGoodLDL());
    assertEquals(false,adult_badLDL.hasGoodLDL());
  }
  public void testHasGoodHDL() {
    assertEquals(true,adult_goodHDL.hasGoodHDL());
    assertEquals(false,adult_badHDL.hasGoodHDL());
  }
}
