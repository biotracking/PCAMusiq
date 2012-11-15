package maxClasses;

import com.cycling74.max.*;

public class ReadFile extends MaxObject
{

	int[][] values;
	float fps;
	int count = 0;
	int frameNum = 0;
			
	private static final String[] INLET_ASSIST = new String[]{
		"inlet 1 help"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"outlet 1 help"
	};
	
	
	public ReadFile(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL});
		
		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);

	}
	
	
	public void bang()
	{	
		
		
		if(frameNum == values[count][0]){
			int[] tempArray = new int[9];

			tempArray[0] = values[count][8];
			tempArray[1] = values[count][7];
			tempArray[2] = values[count][9];
			tempArray[3] = values[count][2];
			tempArray[4] = values[count][3];
			tempArray[5] = values[count][1];
			tempArray[6] = values[count][5];
			tempArray[7] = values[count][6];
			tempArray[8] = values[count][4];

			outlet(0,tempArray);
			count++;
		
		}
			frameNum++;
		//System.out.println(count + "  "+frameNum);
		System.out.println("frame num: "+ frameNum);

	}
    
	public void inlet(int i)
	{
	}
    
	public void inlet(float f)
	{
		
		fps =f;
	}
    
    
	public void list(Atom[] list)
	{
	}
	
	public void setFile(String path){
		
		ReadWriteTextFile file = new ReadWriteTextFile();
		file.readFile(path," ");
		values = file.readItemsInLines();
		System.out.println("Loaded File: "+ path);
		System.out.println(values.length);
		System.out.println(values[15][0]);
		count =0;
		frameNum = 0;
	}
	
	public void reset(){
		count =0;
		frameNum = 0;
		System.out.println("frame num reset to zero!");
	}
	
	
}
