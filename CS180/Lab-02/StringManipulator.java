import java.util.Scanner;

public class StringManipulator{
  
  public String makeUserName(String fullname){
    String str1 = fullname.substring(0, 1);
    int position = fullname.indexOf(" ");
    String str2 = fullname.substring(position + 1);
    String str3 = str1.concat(str2);
    String str4 = str3.toLowerCase();
    
    return str4;
  }
  
  public String makeEmail(String userName, String domain){
    String str5 = domain.toLowerCase();
    String str6 = "@";
    String str7 = str6.concat(str5);
    String str8 = userName.concat(str7);
      
    return str8;
  }
  
  public static void main(String args[]){
    System.out.println("Enter the first name of the person. First name followed by the last name.");
    Scanner fn = new Scanner(System.in);
    String fullname = fn.nextLine();
    System.out.println("Enter the domain");
    String domain = fn.nextLine();
    StringManipulator m = new StringManipulator();
 String ret = m.makeUserName(fullname);
 String met = m.makeEmail(ret, domain);
 System.out.println("The username for the person is:");
 System.out.println(ret);
 System.out.println("The email id for the person is:");
 System.out.println(met);
  }
}
    