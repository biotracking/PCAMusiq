package maxClasses;
import java.io.*;
import java.util.regex.Pattern;

/*
 * Written By Mason Bretan
 * a simple class for reading and writing text files
 */

public class ReadWriteTextFile {
	
	public String readFileName;
	public int numOfLines = 0;
	public int itemsPerLine = 0;
	public String readItemSeparator;
	public String writeFileName;
	private File readFile;
	private FileWriter writeFile;
	private String writeItemSeparator;

	
	public ReadWriteTextFile(){
		
	}
	
	public void readFile(String name, String separator){
		
		readFileName = name;
		readItemSeparator = separator;
		readFile = new File(readFileName);
		setFileInfo(readFile, readItemSeparator);
		
	}
	
	public void writeFile(String name, String separator, int[][] items){
		//use is if there is a delimmiter for an integer array
		writeFileName = name;
		writeItemSeparator = separator;
		
		write(items);
		
	}
	
	public void writeFile(String name, String string){
		
		writeFileName = name;		
		write(string);
		
	}
	
	public void setFileInfo(File name, String REGEX){
		
		try{
			BufferedReader input =  new BufferedReader(new FileReader(name));
			int itemCounter = 0;
			int lineCounter = 0;

			try {
				String line = null; //not declared within while loop

				while (( line = input.readLine()) != null){

					Pattern p = Pattern.compile(REGEX);
					String[] items = p.split(line);
					if(items.length > itemCounter) itemCounter = items.length;
					lineCounter++;
					
				}
			}
			finally {
				input.close();
			} 

			numOfLines = lineCounter;
			itemsPerLine = itemCounter;

		}
		catch (IOException ex){
			ex.printStackTrace();
		}
		
		 
	}
	
	public String readTextFile(){
		String text = "";
		
		return text;
	}
	
	public String[] readLines(){
		
		//returns a String array containing the string on each line
		String[] linesArray = new String[numOfLines];
		if( numOfLines > 0){
			int counter = 0;
			try{
				BufferedReader input =  new BufferedReader(new FileReader(readFile));
				
				try {
					String line = null;
					while (( line = input.readLine()) != null){

						linesArray[counter] = line;
						counter++;

					}
				}
				finally {
					input.close();
				}
			}
			catch (IOException ex){
				ex.printStackTrace();
			}
		}
		return linesArray;
		
	}
	
	public int[][] readItemsInLines(){
		
		//returns an integer array containing the integer values (if they are indeed integers) on each line
		int[][] itemsArray = new int[numOfLines][itemsPerLine];
		if( numOfLines > 0){
			int counter = 0;
			try{
				BufferedReader input =  new BufferedReader(new FileReader(readFile));
				
				try {
					String line = null;
					
					while (( line = input.readLine()) != null){

						Pattern p = Pattern.compile(readItemSeparator);
						String[] items = p.split(line);
						for (int j = 0; j < items.length; j++) {
							//System.out.println(Integer.parseInt(items[j]));
							itemsArray[counter][j] = (int) Double.parseDouble(items[j]);
						}
						counter++;
					}
				}
				finally {
					input.close();
				}
			}
			catch (IOException ex){
				ex.printStackTrace();
			}
		}
		return itemsArray;
		
	}

	
	public void write(int[][] items){
		//writes an array to the text file
		
		try {
			writeFile = new FileWriter(writeFileName);
			BufferedWriter writer = new BufferedWriter(writeFile);
			String line;
			
			for(int i=0; i<items.length; i++){
				line = "";
				for (int j=0; j<items[i].length; j++){
					if(j < items[i].length-1){
						line = line + Integer.toString(items[i][j]) + writeItemSeparator;
					}else{
						line = line + Integer.toString(items[i][j]);
					}
				}
				writer.write(line);
				writer.newLine();
			}
			writer.close();
			
		} catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void write(String string){
		//writes a string to the text file
		
		try {
			writeFile = new FileWriter(writeFileName);
			BufferedWriter writer = new BufferedWriter(writeFile);
			writer.write(string);
			writer.close();
			
		} catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void appendLine(String line){
		
		boolean append = true; 
		try{
			FileWriter fw = new FileWriter(writeFileName,append); 
			fw.write(line);//appends the string to the file 
			fw.close(); 
		} catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void appendLine(String name, String line){
		
		writeFileName = name;
		boolean append = true; 
		try{
			FileWriter fw = new FileWriter(writeFileName,append); 
			fw.write(line);//appends the string to the file 
			fw.close(); 
		} catch (IOException e){
			e.printStackTrace();
		}
	}
	
	public void setReadFile(String path){
		readFileName = path;
		readFile = new File(readFileName);		
	}
	
	public void setWriteFile(String path){
		writeFileName = path;
	}
}
