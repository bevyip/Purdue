/**
*CS180 - Lab00
*This is a program that will be used to calculate the Hypotenause of a triangle using the pythagoras theorem
*Shreyaansh Bassi, sbassi@purdue.edu
*/
import java.util.Scanner;
public class Pythagoras {
  public static void main(String[] args) {
    Pythagoras p = new Pythagoras();
    Scanner scanner = new Scanner(System.in);
    System.out.print("Enter side 'a': ");
    int a = scanner.nextInt();
    System.out.print("Enter side 'b': ");
    int b = scanner.nextInt();
    double h = p.getHypotenause(a, b);
    System.out.println("Hypotenause: " + h);
  }
  
  public double getHypotenause(int a, int b) {
    double result;
      result= Math.sqrt(a*a+b*b);
      return result; 
  }
}

    
