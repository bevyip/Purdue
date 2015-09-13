import java.util.Scanner;
import java.util.Random;

public class Minesweeper {
    
    private final boolean[][] minefield;
    private final int [][] clueGrid;
    private final boolean[][] checked;
    private int rows;
    private int columns;
    
    public Minesweeper(int rows, int columns, int mines) {
        this.rows = rows;
        this.columns = columns;
        minefield = new boolean[rows][columns];
        clueGrid = new int[rows][columns];
        checked = new boolean[rows][columns];
        generateMinefield(mines);
        generateClueGrid();
        
        for (int i = 0; i < checked.length; i++)
            for (int j = 0; j < checked[i].length; j++)
            checked[i][j] = false;
    }
    
    private void generateMinefield(int mines) {
        for (int i = 0; i < minefield.length; i++) {
            for (int j = 0; j < minefield[i].length; j++) {
                minefield[i][j] = false;
            }
        }
        
        Random random = new Random(System.currentTimeMillis());
        while (mines > 0) {
            int r = random.nextInt(minefield.length);
            int c = random.nextInt(minefield[r].length);
            if (!minefield[r][c]) {
                minefield[r][c] = true;
                mines--;
            }
        }
    }
    
    private void generateClueGrid() {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
            clueGrid[i][j] = minefield[i][j] ? -1 : countMines(i,j);
    }
    
    private int countMines(int row, int column) {
        int n = 0;
        for (int i = Math.max(row-1,0); i <= Math.min(row+1,rows-1); i++)
            for (int j = Math.max(column-1,0); j <= Math.min(column+1,columns-1); j++)
            if (!(i == row && j == column))
            n += minefield[i][j] ? 1 : 0;
        return n;
    }
    
    private void printBoard() {
        System.out.println("rows:"+rows+"col:"+columns);
        // print column numbers
        System.out.print("   "); // 3 spaces
        for (int j = 0; j < columns; j++) // for each column
            System.out.print(" " + (j+1)); // number 1 to columns
        System.out.println(); // print newline
        // print dividing line
        System.out.print("  +-"); // 2 spaces, a plus, and a dash
        for (int j = 0; j < columns; j++) // for each column
            System.out.print("--"); // 2 dashes
        System.out.println(); // print newline
        // print row letters and board cells
        char row_letter = 'A';
        for (int i = 0; i < rows; i++) {
            // print a row of cells
            System.out.print(row_letter + " |"); // row letter, 1 space, vertical pipe
            for (int j = 0; j < columns; j++) { // for each cell in the row
                char cell_symbol;
                if (!checked[i][j]) // unchecked cell
                    cell_symbol = '?';
                else if (minefield[i][j]) // mine
                    cell_symbol = '*';
                else if (clueGrid[i][j] > 0) // positive clue
                    cell_symbol = (char)('0' + clueGrid[i][j]);
                else  // 0 clue (or clue < -1, which is funky, so we'll ignore it and print a space)
                    cell_symbol = ' ';
                System.out.print(" " + cell_symbol);  // 1 space and cell symbol
            }
            System.out.println(); // print newline
            row_letter++;
        }
    }
    
    public void start() {
        Scanner scanner = new Scanner(System.in);
        boolean win = false; // have not won yet
        while (!win) {
            // show board to user
            printBoard();
            // prompt user for cell to check
            System.out.print("Check cell? ");
            // get input from user
            String line = scanner.nextLine().toUpperCase();
            int row = line.charAt(0)-'A';
            int column = line.charAt(1)-'1';
            // check the cell
            if (minefield[row][column]) // game over, loss
                break;
            else // game continues 
                // mark cell as checked
                checked[row][column] = true;
            // if all unchecked cells are mines, game is won
            win = true;
            for (int i = 0 ; i < rows && win; i++)
                for (int j = 0; j < columns && win; j++)
                if (!checked[i][j] && !minefield[i][j])
                win = false; // unchecked cell is not a mine
        }
        // game is over
        scanner.close(); // close the scanner
        // set all cells to checked
        for (int i = 0 ; i < rows; i++)
            for (int j = 0; j < columns; j++)
            checked[i][j] = true;
        // show board to user
        printBoard();
        if (win) // congratulate user on success
            System.out.println("\nYou win!");
        else // scold or console user for loss
            System.out.println("\nYou're not very good at this are you?");
    }
    public static void main(String[] args) {
        Minesweeper m = new Minesweeper(3, 3, 1);
        m.start();
    }
}