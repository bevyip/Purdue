/**
 * CS 180 - Lab 08 - CollegeFeeCalculator
 * 
 * (Brief description of the class file)
 * 
 * @author Shreyaansh Bassi sbassi@purdue.edu
 * 
 * @lab (Your lab section)
 *
 * @ 11/05/14
 */

import javax.swing.JOptionPane;
import javax.swing.ImageIcon;

public class CollegeFeeCalculator {
    public static void main(String[] args) {
        int tuitionFee = 0;
        int housingExpense = 0;
        int totalFee = 0;
        String res = "";
        String residence = "";
        int c = 0;
        String house = "";
        int res1 = 0;
        boolean valid = false;
        int enroll = 0;
        int creditHours = 0;
        String[] enrollment = {"Part-time", "Full-time"};
        String[] residency = {"In-State", "Out-of-State", "International"};
        JOptionPane.showMessageDialog(null, "Welcome to College Fee Calculator", "College Fee Calculator", JOptionPane.INFORMATION_MESSAGE);
        do {
            String name = JOptionPane.showInputDialog(null, "Please enter your name, then press OK", "Name", JOptionPane.QUESTION_MESSAGE);
            if (name == null || name == "") {
                return;
            }
            do {
                enroll = JOptionPane.showOptionDialog(null, "Please select your enrollment", "Enrollment", JOptionPane.DEFAULT_OPTION, JOptionPane.OK_CANCEL_OPTION, null, enrollment, enrollment[1]);
                String credits = JOptionPane.showInputDialog(null, "Please enter the no. of credit hours, then press OK", "Credit Hours", JOptionPane.QUESTION_MESSAGE);
                creditHours = Integer.parseInt(credits);
                if ((creditHours < 8 && enroll == 0) || (creditHours >= 8 && enroll == 1)) {
                    valid = true;
                } else if ((creditHours >= 8 && enroll == 0) || (creditHours < 8 && enroll == 1)) {
                    JOptionPane.showMessageDialog(null, "Please enter valid credit hours for the current enrollment", "Invalid no. of credits", JOptionPane.ERROR_MESSAGE);
                }
            } while (!valid);
            res = (String)JOptionPane.showInputDialog(null, "Please select the appropriate residency", "Residency", JOptionPane.DEFAULT_OPTION, null, residency, residency[2]);
            String[] housing = {"On-campus", "Off-campus"};
            house = (String)JOptionPane.showInputDialog(null, "Please select your housing", "Housing", JOptionPane.PLAIN_MESSAGE, null, housing, housing[1]);
            if (house.equals("On-campus")) {
                String[] residenceHall = {"Earhart", "Hillenbrand", "Owen", "Windsor"};
                residence = (String)JOptionPane.showInputDialog(null, "Please select the residence hall", "Residence Hall", JOptionPane.PLAIN_MESSAGE, null, residenceHall, residenceHall[3]);
            }
            if (enroll == 0) {
                if (res.equals("In-State")) {
                    tuitionFee = (350)*creditHours;
                } else if (res.equals("Out-of-State")) {
                    tuitionFee = (350 + 600)*creditHours;
                } else if (res.equals("International")) {
                    tuitionFee = (350 + 600 + 70)*creditHours;
                }
            } else { 
                if (enroll == 1) {
                    if (res.equals("In-State")) {
                        tuitionFee = 4996;
                    } else if (res.equals("Out-of-State")) {
                        tuitionFee = 4996 + 9401;
                    } else if (res.equals("International")) {
                        tuitionFee = 4996 + 9401 + 1000;
                    }
                }
            }
            if (house.equals("On-campus")) {
                if (residence.equals("Earhart")) {
                    housingExpense = 4745;
                }
                if (residence.equals("Hillenbrand")) {
                    housingExpense = 5307;
                }
                if (residence.equals("Owen")) {
                    housingExpense = 4130;
                }
                if (residence.equals("Windsor")) {
                    housingExpense = 4150;
                }
            } else {
                if (house.equals("Off-campus")) {
                    housingExpense = 0;
                }
            }
            totalFee = tuitionFee + housingExpense;
            JOptionPane.showMessageDialog(null, "<html> Name: " + name + "<br>Credit Hours: " + creditHours + "<br>Enrollment: " + enrollment[enroll]
                                              + "<br>Residency: " + res + "<br>Tuition Fee: " + tuitionFee + "<br>Housing Expenses: " + housingExpense
                                              + "<br>Total Sem. Fee: " + totalFee + "</html>");
            c = JOptionPane.showConfirmDialog(null, "Would you like to perform another fee calculation?", "Are you done?", JOptionPane.YES_NO_OPTION);
            
        } while (c == 0);
    }
}