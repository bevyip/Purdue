import java.util.*;
import java.sql.*;

public class StudentDB {
	Connection con;
	
	public StudentDB() {
		try {
			Class.forName( "oracle.jdbc.driver.OracleDriver" );
		}
		catch ( ClassNotFoundException e ) {
			e.printStackTrace();
		}
		try {
			con =
					DriverManager.getConnection( "jdbc:oracle:thin:@claros.cs.purdue.edu:1524:strep","USERNAME", "PASSWORD" );
		}
		catch ( SQLException e ){
			e.printStackTrace();
		}
	}

	/* Simple SQL statement: Names of students in department */
	public void getStudentsInDepartment( int department ) {
		String query = "Select sname from Student where deptid=" + department;
		try {
			Statement stmt = con.createStatement();
			ResultSet rs = stmt.executeQuery( query );
			while ( rs.next() ) {
				String name = rs.getString( "sname" );
				System.out.println( name );
			}
			rs.close();
			stmt.close();
		}
		catch ( SQLException e ) {
			e.printStackTrace();
		}
	}

	/* Prepared Statement: Names of students in department */
	public void getStudentsInDepartmentPrepared ( int department ) {
		String stmt = "Select sname from Student where deptid=?";
		try {
			PreparedStatement ps = con.prepareStatement( stmt );
			ps.setInt( 1, department );
			ResultSet rs = ps.executeQuery();
			while ( rs.next() ) {
				String sname = rs.getString( "sname" );
				System.out.println( sname );
			}
			rs.close();
			ps.close();
		}
		catch (SQLException e){}
	}

	/* Nested Query: Names of students enrolled in a course */
	public void getStudentsInClass( String cname ) {
		String query = "Select snum from Enrolled where cname = '" + cname + "'";
		try {
			Statement stmt = con.createStatement();
			ResultSet rs = stmt.executeQuery( query );
			while ( rs.next() ) {
				int snum = rs.getInt( "snum" );
				String nestedQuery = "Select sname from Student where snum=" + snum;
				Statement stmtNested = con.createStatement();
				ResultSet rsNested = stmtNested.executeQuery( nestedQuery );
				while ( rsNested.next() ) {
					String sname = rsNested.getString( "sname" );
					System.out.println( sname );
				}
				rsNested.close();
				stmtNested.close();
			}
			rs.close();
			stmt.close();
		}
		catch ( SQLException e ) {}
	}

	/* Update: Insert data into database */
	public void enroll( int snum, String cname, String grade ) {
		String update = "insert into Enrolled values(" + snum + ",'" + cname +"','"+grade+"')";
		System.out.println(update);
		try {
			Statement stmt = con.createStatement();
			stmt.executeUpdate( update );
			stmt.close();
		}
		catch ( SQLException e ) {}
	}

	/* Update: Insert user input into database */
	public void insertNewDepartment() {
		try {
			Scanner scanner = new Scanner(System.in);
			System.out.print("Enter department id:");
			int did = scanner.nextInt();
			scanner.nextLine();
			System.out.print("Enter department name:");
			String depName = scanner.nextLine();
			System.out.print("Enter department location:");
			String depLocation = scanner.nextLine();

			String insertDepartment = "insert into Department " + " (deptid, dname, location)" + " values (?, ?, ?)";
			PreparedStatement stmt = con.prepareStatement(insertDepartment);
            stmt.setInt(1, did);
            stmt.setString(2, depName);
            stmt.setString(3, depLocation);
            stmt.executeUpdate();
            stmt.close();
     	}
     	catch(SQLException e){}
	}

	/* Main() */
	public static void main( String [] args ) {
		StudentDB sdb = new StudentDB();
		sdb.getStudentsInDepartment(11);
		System.out.println( "-----------------" );
		sdb.getStudentsInDepartmentPrepared(11);
		System.out.println( "-----------------" );
		sdb.enroll( 4181, "ENG40000","A" );
		System.out.println( "-----------------" );
		sdb.insertNewDepartment();
	}
}

//select table_name from user_tables;