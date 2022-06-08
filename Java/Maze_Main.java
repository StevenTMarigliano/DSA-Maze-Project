/**
 * @author Steven T Marigliano
 *
 */
import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle file input errors
import java.util.InputMismatchException; // Import this class to handle input mismatch errors
import java.util.Scanner; // Import the Scanner class to read text files
import java.util.List;
import java.util.ArrayList;

public class Maze_Main {
    public static int rows, columns, curr_row, curr_column, total_rows, total_columns, prev_row, prev_column;
 
    public static int[][] maze; // 2d array to house mouse
    public static List<Integer> rows_traversed= new ArrayList<>(); // array list to keep track of rows that have been traversed
    public static List<Integer> columns_traversed = new ArrayList<>(); // array list to keep track of columns I have traversed
    public static boolean solved;
    
    //read in the maze text file
    public static void read_file(String str){
        try {
            File myObj = new File(str);
            Scanner fileReader = new Scanner(myObj);
            List<Integer> integers = new ArrayList<>();
            while(fileReader.hasNext()){
                if(fileReader.hasNextInt()){
                    integers.add(fileReader.nextInt());
                }else{
                    fileReader.next();
                }
            }
            
            rows = integers.get(0);
            columns = integers.get(1);
            total_rows = rows - 1;
            total_columns = columns - 1;
            maze = new int[rows][columns];
            
            for(int i = 2; i < integers.size(); i += columns){
                for(int r = 0; r < rows; r++){
                    for(int c = 0; c < columns; c++){
                        maze[r][c] = integers.get(i);
                        i++;
                    }
                }
            }
            for(int r = 0; r < rows; r++){
                  System.out.println();
                    for(int c = 0; c < columns; c++){
                        System.out.printf("%d ", maze[r][c]);
                    }
            }

            System.out.printf("\nRow & Column Start At(0,0)\n\n");
            
            fileReader.close();
            
        } catch (FileNotFoundException e) {
            System.out.println("Invalid file name.\n");
        }
    }
    
    //check cell to the right
    public static boolean traverse_right(){
        if(curr_column + 1 > total_columns){
            return false;
        }else if(maze[curr_row][curr_column + 1] == 1){
            return false;
        }else if(maze[curr_row][curr_column + 1] == 2){
            return false;
        }else if(maze[curr_row][curr_column + 1] == 3){
            return false;
        }else {
            prev_row = curr_row;
            prev_column = curr_column;
            return true;
        }
    
    }
    //check cell to the left    
    public static boolean traverse_left(){
        if(curr_column - 1 == -1){
            return false;
        }else if(maze[curr_row][curr_column - 1] == 1){
            return false;
        }else if(maze[curr_row][curr_column - 1] == 2){
            return false;
        }else if(maze[curr_row][curr_column - 1] == 3){
            return false;
        }else{
            prev_row = curr_row;
            prev_column = curr_column;
            return true;
        }
        
    }
    //check cell above
    public static boolean traverse_up(){
        if(curr_row - 1 == -1){
            return false;
        }else if(maze[curr_row - 1][curr_column] == 1){
            return false;
        }else if(maze[curr_row - 1][curr_column] == 2){
            return false;
        }else if(maze[curr_row - 1][curr_column] == 3){
            return false;
        }else{
            prev_row = curr_row;
            prev_column = curr_column;
            return true;
        }
    
    }
    //check cell below
    public static boolean traverse_down(){
        if(curr_row + 1 > total_rows){
            return false;
        }else if(maze[curr_row + 1][curr_column] == 1){
            return false;
        }else if(maze[curr_row + 1][curr_column] == 2){
            return false;
        }else if(maze[curr_row + 1][curr_column] == 3){
            return false;
        }else{
            prev_row = curr_row;
            prev_column = curr_column;
            return true;
        }
    }
    
    //backtrack to previous cell with intersection
    public static void backtrack(){
        maze[curr_row][curr_column] = 3;
        rows_traversed.remove(rows_traversed.size() - 1);
        columns_traversed.remove(columns_traversed.size() - 1);
        curr_row = prev_row;
        curr_column = prev_column;
        prev_row = rows_traversed.get(rows_traversed.size() -2);
        prev_column = columns_traversed.get(columns_traversed.size() -2);

    }
    
    // main function: traverses through maze in any direction that returns true 
    //                otherwise it backtracks
    public static void traverse_maze(){
       if(traverse_right() == true){
                maze[curr_row][curr_column] = 2;
                curr_column++;
                rows_traversed.add(curr_row);
                columns_traversed.add(curr_column);
       }else if(traverse_left() == true){
                maze[curr_row][curr_column] = 2;
                curr_column--;
                rows_traversed.add(curr_row);
                columns_traversed.add(curr_column);
       }else if(traverse_up() == true){
                maze[curr_row][curr_column] = 2;
                curr_row--;
                rows_traversed.add(curr_row);
                columns_traversed.add(curr_column);
       }else if(traverse_down() == true){
                maze[curr_row][curr_column] = 2;
                curr_row++;
                rows_traversed.add(curr_row);
                columns_traversed.add(curr_column);
        }else{
            backtrack();
        }
    }

    //prints the current state of the maze
    public static void print_maze(){
        //Print final Maze Solution
        for(int r = 0; r < rows; r++){
            System.out.println();
            for(int c = 0; c < columns; c++){
                if(maze[r][c] == 0 || maze[r][c] == 3){
                    System.out.print("0 ");
                }else if(maze[r][c] == 2){
                    System.out.print("  ");
                }else{
                    System.out.printf("1 ");
                }
            }
        }
        System.out.println();
    }

    public static void menu(){
        int stop = 5;
        //create scanner
        String maze_name = "";
        Scanner sc = new Scanner(System.in); 
        
        while(stop != 0){
        solved = false;
         System.out.printf("Maze Traversal\nUse following commands to operate\n0 - End Program\n1 - Load Maze\n2 - Enter Entry Point\n3 - Load Maze\nEnter Command(0-3): ");
          
         try
         {
            stop = sc.nextInt();
            sc.nextLine();
         }
         catch(InputMismatchException e) {
            System.out.printf("\nNot a valid entry.\n");
            sc.nextLine();
        }
 
         if (stop == 0){
             break;
         }else if(stop == 1){
             System.out.printf("       Select a maze:\nmaze1.txt     Dimension: 10x10\nmaze2.txt     Dimension: 10x10\nmaze3.txt     Dimension: 10x10\nmaze4.txt     Dimension: 20x20 \nmaze5.txt     Dimension: 31x31\n\nEnter a maze: ");
             maze_name = sc.nextLine();
             read_file(maze_name);
         }else if (stop == 2 && maze_name != ""){
             System.out.printf("\nEnter Starting Row: ");

                try {
                    curr_row = sc.nextInt(); 
                    sc.nextLine();
                }catch (InputMismatchException e) {
                    System.out.printf("\nNot a valid entry.\n");
                    sc.nextLine();
                }

                System.out.printf("\nEnter Starting Column: ");
                
                try{
                    curr_column = sc.nextInt();
                    sc.nextLine();
                }
                catch(InputMismatchException e)
                {
                    System.out.printf("\nNot a valid entry.\n");
                    sc.nextLine();
                }

              

                if(curr_row > total_rows || curr_row < 0 || curr_column > total_columns || curr_column < 0){
                    System.out.printf("\nNot a valid entry. Try again\n\n");
                }else if(maze[curr_row][curr_column] == 1){
                    System.out.printf("\nCan't enter there. Try again.\n\n");
                }else{
                    rows_traversed.add(curr_row);
                    columns_traversed.add(curr_column);
                    
                    while(solved == false){
                        traverse_maze();
                    
                        if(curr_row == total_rows || curr_column == total_columns || curr_row == 0 || curr_column == 0 ){
                            maze[curr_row][curr_column] = 2;
                            solved = true;
                            print_maze();
                            System.out.printf("\n\n");
                        }
                    }
                }
        }else if (stop == 3){
            print_maze();
        }else{
             System.out.println("\nNot a valid entry or you didn't select a maze. Try again.\n");
         }
     }
     sc.close();
   }
    
  public static void main(String[] args) {
        menu();

         /*prints current state of maze
        for(int r = 0; r < rows; r++){
            System.out.println();
            for(int c = 0; c < columns; c++){
                System.out.printf("%d ", maze[r][c]);
            }
         }
        */    
         /* prints final solution (rows, columns)
        System.out.println();
        System.out.printf("Rows: %s\nColumns: %s\n", rows_traversed, columns_traversed );
    */
    }
    


}
