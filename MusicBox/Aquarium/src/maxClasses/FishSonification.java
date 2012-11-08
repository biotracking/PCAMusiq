package maxClasses;


import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
//import java.util.Random;
import java.util.regex.Pattern;
import java.util.*;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.DataLine;

import com.cycling74.max.*;

public class FishSonification extends MaxObject
{
	
	//outlet constants
	public static final int SAMPLE1 = 0;
	public static final int SAMPLE1_STOP = 1;
	public static final int SAMPLE2 = 2;
	public static final int SAMPLE2_STOP = 4;
	public static final int SAMPLE2_START = 3;
	public static final int SAMPLE3 = 6;
	public static final int SAMPLE3_STOP =8;
	public static final int SAMPLE3_START = 7;
	public static final int SPATIALIZE1 = 10;
	public static final int VOLUME1 = 11;
	
	public static final int[][] FADEVOLUMES =  {{5,9},{ 15,19}};
	//public static final int MELODY_OUTLET = 8;
	
	public static final int Y_SCREEN_SIZE  = 640;
	public static final int X_SCREEN_SIZE= 440;
	
	public static final int SAMPLE4 = 12;
	public static final int SAMPLE4_START = 13;
	public static final int SAMPLE4_STOP = 14;
	public static final int SAMPLE5 = 16;
	public static final int SAMPLE5_START =17;
	public static final int SAMPLE5_STOP =18;
	public static final int SPATIALIZE2 =20;
	public static final int VOLUME2 = 21;
	
	public static final int DRUM_VOLUME = 22;
	public static final int DRUM_HITS = 23;
	public static final int SPATIALIZE_DRUM = 24;
	public static final int DRUM_TEMPO = 25;
	
	private int noteDensity = 2;
	private int phraseLength = 2;
	private int timbre = 1;
	private int chordality = 2;
	private int register = 1;
	private int volume = 1;
	private String homeDirectory;
	private boolean sampleExists;
	private String selectedSample;
	private String sampleString;
	private boolean overallSamplePlaying = false;
	private boolean[] singleFishSample = new boolean[2];
	private boolean[] fadingOutSample = new boolean[2];
	private boolean[] fadingInSample = new boolean[2];
	private float[][] fadeVolumes = new float[2][2];
	private boolean[][] sampleInitiated = new boolean[2][2];
	private boolean[][] interFadeOut = new boolean[2][2];
	private boolean[] generatingMelody = new boolean[3];
	private int noteFileNum;
	private String currentPlayingSample;
	private boolean changeWarranted;
	private long[] fileLength = new long[4];
	private long[][] sampleStartTime = new long[2][2];
	private int[] repetitionCount = new int[2];
	public float[] sampleVolume = new float[2];
	
	//****variables from vision***
	//overall
	private float activity;
	private float density;
	private float excitement;
	private float[] previousActivity = new float[2];
	private float[] previousDensity = new float[2];
	private float[] previousExcitement = new float[2];
	
	//individual fish
	private float fish1X;
	private float fish1Y;
	private float interaction;
	private float size1;
	private float degreeMovement;
	private float unexpectedMovement;
	private float[] previousSize1 = new float[2];
	private float fish2X;
	private float fish2Y;
	private float size2;
	private float[] previousSize2 = new float[2];
	private int fish1SizeLevel;
	private int fish2SizeLevel;
	
	private float[] sizes1 = new float[10];
	private float[] sizes2 = new float[10];
	private int frameCount = 0;
	
	//priority of importance between single fish or overall scene
	private float priority;
	private float[] previousPriority = new float[2];
	
	private float realTimeNoteDensity = .6f;
	
	
	//*****drum variables******
	private int drumTempo;
	private int[][][] drumPattern = new int[10][16][2];  //[x][x][0] = hits and [x][x][1] = accents
	private int hitNum;
	//*probability templates*
	private static final float[] BASSDRUM = {1,0,.5f,0,.4f,0,0,0,1,0,.5f,0,1,0,0,0};
	private static final float[] SNARE ={0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
	private static final float[] BONGO2 = {.8f, .2f, .2f, 0, .8f, 0, .5f,0, .5f, 0, .5f, 0, 0,0,0,0};
	private static final float[] BONGO1 = {.8f, 0, 0, 0, .8f, 0, 0,.5f, 0, 0, .5f, .5f, 0,.3f,.5f,.5f};
	private static final float[] FLOORTOM = {1, .8f, .6f, .6f, 0, .5f, .5f, .5f, 1,.6f, .7f,.6f, 0, .2f, .8f,.6f};
	private static final int[] DRUMVOLUMES = {132, 100, 110,120, 118, 100, 100, 100, 100, 110};
	
	
	// laurie anderson patterns
	private static final float[] LABASSDRUM = {1.f, .8f, 8.f,0,0,0,0, 1.f, .8f , .8f,0,0,0,0,.8f,0};
	private static final float[] LASNARE = {0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0};
	private static final float[] LAFLOORTOM = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,0,0,0};
	private boolean laurieDrumPlaying = false;
	
	
	
	
	//*****MARKOV VARIABLES***********
	
	//notes
	int TRANSCRIPTIONS = 20;
	private int[][][] notes = new int[TRANSCRIPTIONS][50][2];
	private int[] totalNotes = new int[TRANSCRIPTIONS];
	private int[] firstOrderLowerLimit = new int[TRANSCRIPTIONS];
	private int[] secondOrderLowerLimit = new int[TRANSCRIPTIONS];
	private final int lowestMidiValue = 36; //lowest note shimon can play
	//general
	private final int peakNote = 100; 
	private int markovOrderNum = 0;
	private int bassMarkovOrderNum = 0;
	private int previousNotePlayed;
	private int totalMelodies = TRANSCRIPTIONS;

	
	//single order note midi value
	private float[][][] transitionNoteMatrix_1 = new float[TRANSCRIPTIONS][peakNote][peakNote];
	private final int totalNoteOptions = 100;
	private int[][][] totalNoteTransitions = new int[TRANSCRIPTIONS][peakNote][peakNote];	//the total number of transitions possible to make from note A to note B
	private int[][] totalOccurrences = new int[TRANSCRIPTIONS][peakNote];  //the total number of times a single note is played
	
	
	//single order duration midi values
	private float[][][][] transitionDurMatrix_1 = new float[TRANSCRIPTIONS][peakNote][peakNote][8];
	private int[][][] noteLength = new int[peakNote][peakNote][8];  //each duration of the note played before it makes it's transition (8 different types, sixteenth, triplet, eighth, dotted eighth, quarter, etc...whole)
	private final float[] possibleNoteDurations = {(float)62.5, (float)83.33, 125, (float)187.5, 250, 375, 500, 750, 1000};  //the note durations possible
	private final int totalDurationOptions = 8;
	//private static int[][][] totalDurTransitions = new int[100][100][8];
	private int[][][] totalDurationTransitionOccurrences = new int[TRANSCRIPTIONS][peakNote][peakNote]; //the number of occurrences that a particular transition takes place (i.e. midi note 40 to 42)
	private int[][][][] totalDurationTypeTransitionOccurrences = new int [TRANSCRIPTIONS][peakNote][peakNote][8];//the number of occurrences that a particular rhythmic movement connects two notes (i.e. a quarter note connects 40 to 42)
	
	

	private static final String[] INLET_ASSIST = new String[]{
		"inlet 1 help"
	};
	private static final String[] OUTLET_ASSIST = new String[]{
		"outlet 1 help"
	};
	
	public FishSonification(Atom[] args)
	{
		declareInlets(new int[]{DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL});
		declareOutlets(new int[]{DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL, DataTypes.ALL, DataTypes.ALL, DataTypes.ALL, DataTypes.ALL, DataTypes.ALL, DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL,DataTypes.ALL});
		
		setInletAssist(INLET_ASSIST);
		setOutletAssist(OUTLET_ASSIST);
		
		//****outlets*****
		//overall
		//0 = sample 1 string
		//1 = sample 1 stop
		
		//individual fish 
		//2 = sample 2 string
		//3 = sample 2 stop
		//4 = sample 3 string
		//5 = sample 3 stop
		//6 = sample 2,3 x position
		//7 = sample 2,3 y position
		
		//8 = sample 4 string
		//9 = sample 4 stop
		//10 = sample 5 string
		//11 = sample 5 stop
		//12 = sample 4,5 x position
		//13 = sample 4,5 y position
		
		//drum stuff
		//14 = array for volumes of specific drums
		//15 = array for hit patterns
		//16 = spatialization and overall volume array
		//17 = tempo

		
		//loadNoteData();
	}
    
	public void bang()
	{
		int inlet = getInlet();
		
		switch(inlet){
		case 0:
			//sampleExists = getSampleFromData(volume, phraseLength, register, 1);
		
			if(sampleExists){
				outlet(0, selectedSample);
			}else{
				
				if(checkAltVolumeSetting() == true){
					outlet(0, selectedSample);
				}else{					
					generatePhrase();
				}
				
			}
			break;
		case 1:

			//if(shouldPlayNote(realTimeNoteDensity)){
			if(shouldPlayNote(realTimeNoteDensity)){
				int temp;
				String noteFile;
				temp = getNextFirstOrderNote(previousNotePlayed, 0);
				previousNotePlayed = temp;
				/*temp = verifyNote(temp, noteFileNum);
				noteFile = "samples/IndividualNotes/note" + Integer.toString(noteFileNum) + "/"+Integer.toString(temp)+".wav";
				noteFile = homeDirectory.concat(noteFile);
				outlet(3, noteFile);
				*/
				outlet(0,temp);
			}
			break;
			
		case 2:  //drum bangs
			
			int hits[] = new int[10];
			int accents[] = new int[10];
			
			if(hitNum < 16){
				
				for(int i =0; i < 10; i++){
					accents[i] = drumPattern[i][hitNum][1];
					hits[i] = drumPattern[i][hitNum][0];
				}		
				hitNum++;
				
				outlet(DRUM_VOLUME, accents);
				outlet(DRUM_HITS, hits);
			}else{
				//generate new measure
				//createDrumPattern();
				
				if(laurieDrumPlaying){
					createLaurieDrumPattern();
				}
			}
			
			
			
		}

	}
    
	public void inlet(int i)
	{


	}

	public void inlet(float f)
	{


	}


	public void list(Atom[] list)
	{

		int inlet = getInlet();

		if(inlet == 0){
			int melodyNum = list[2].getInt();

			notes[melodyNum][totalNotes[melodyNum]][0] = list[0].getInt();
			notes[melodyNum][totalNotes[melodyNum]][1] = list[1].getInt();
			totalNotes[melodyNum]++;
		}else if(inlet == 1){

			for(int i = 0; i < 9;i ++){
				switch(i){
				case 0:
					density = list[i].getInt()/127f;
					break;
				case 1:
					if(previousActivity[0] != activity){
						previousActivity[1] = previousActivity[0];
						previousActivity[0] = activity;
					}
					activity = list[i].getInt()/127f;
					break;
				case 2:
					previousExcitement[1] = previousExcitement[0];
					previousExcitement[0] = excitement;
					excitement = list[i].getInt()/127f;
					break;
				case 3:
					//interaction = list[i].getInt()/127f;
					fish1X= list[i].getInt()/127f;
					break;
				case 4:
					
					fish1Y= list[i].getInt()/127f;
					break;
				case 5:
					
					previousSize1[1] = previousSize1[0];
					previousSize1[0] = size1;
					size1 = list[i].getInt()/127f;
					//sizes1[frameCount] = size1;
					break;
				case 6:
					fish2X= list[i].getInt()/127f;
					break;
				case 7:					
					fish2Y= list[i].getInt()/127f;
					break;
				case 8:
					previousSize2[1] = previousSize2[0];
					previousSize2[0] = size2;
					size2 = list[i].getInt()/127f;
					
					break;
				}

			}


			setSpatializationAndVolume();
			adjustInterFades();
			getFishSizeLevels();
			analyzeValues();
			//setDrumTempo();
			//setDrumVolandSpace();
			
			
			//setLaurieDrum();


		}

	}
	
	
	private void getFishSizeLevels(){
		
		System.out.println(size1 + "   "+ size2);
		
		if(size1 > .8){
			fish1SizeLevel = 5;
		}else if(size1 < .8 && size1 > .61){
			fish1SizeLevel = 4;
		}else if(size1 < .61 && size1 > .51){
			fish1SizeLevel = 3;
		}else if(size1 <.51 && size1 > .32){
			fish1SizeLevel = 2;
		}else if(size1 < .32 && size1 >0){
			fish1SizeLevel = 1;
		}else{
			fish1SizeLevel = -1;
		}
		
		if(fish1SizeLevel < 5 && fish1SizeLevel >1){
			fish1SizeLevel += 1;
		}
		
		if(size2 > .8){
			fish2SizeLevel = 5;
		}else if(size2 < .8 && size2 > .61){
			fish2SizeLevel = 4;
		}else if(size2 < .61 && size2 > .51){
			fish2SizeLevel = 3;
		}else if(size2 <.51 && size2 > .32){
			fish2SizeLevel = 2;
		}else if(size2 < .32 && size2 > 0){
			fish2SizeLevel = 1;
		}else{
			fish2SizeLevel = -1;
		}
		if(fish2SizeLevel < 5 && fish2SizeLevel >1){
			fish2SizeLevel += 1;
		}/*else if(fish2SizeLevel == 1){
			fish2SizeLevel = -1;
		}*/
		
		
	}
	
	private void analyzeValues(){
		
		//noteDensity
		//phraseLength
		//timbre
		//chordality
		//register
		//volume
		
		//getting the overall parameters	
		/*if(overallSamplePlaying == false){
			noteDensity = 1;
			phraseLength = 3;
			timbre = 1;
			chordality = 2;
			volume = 1;
			if(previousActivity[1] > activity){
				register = 3;
			}else if(previousActivity[1] > activity){
				register = 1;
			}else{
				register = 2;
			}

			setOverallSample();

		}*/
		
		
		if(previousExcitement[0] < excitement){
			realTimeNoteDensity = .6f;
		}else{
			realTimeNoteDensity = .15f;
		}
		
		interpretValues(1, size1);
		interpretValues(2, size2);
	}
	
	
	private void interpretValues(int fishNum, float tempSize){
		
		if(tempSize > 0){
			if(singleFishSampleIsPlaying(fishNum) == true){				
						//check if it needs to be changed
						if(withinSizeThreshold(fishNum)){
							return;
						}else{
							System.out.println("size threshold exceeded");
							if(fadingOutSample[fishNum-1] == false){
								turnOffSingleFishSamples(fishNum);
							}						
						}

			}else{
				System.out.println("1");
				updateSingleFishSample(fishNum);
			}
				
		}else{
			if(withinPriorityThreshold(fishNum)){
				if(singleFishSampleIsPlaying(fishNum) && fadingOutSample[fishNum-1] == false){
					turnOffSingleFishSamples(fishNum);
					System.out.println("single fish sample off - " + fishNum);
				}
			}
			
			if (generatingMelody[0] == true){
				stopGeneratingMelody();
			}
			
		}
		
	}
	
	private void adjustGains(){
		if(singleFishSample[0] == true || generatingMelody[0] == true){
			outlet(8, 127);
		}else{
			outlet(8,127);
		}
	}
	
	private void turnOffSingleFishSamples(int fishNum){
		
		//triggers a fade out and when fade out is finished sample is turned off and variables changed to false
		if(fishNum == 2){
			fadingOutSample[1] = true;
			//outlet(SAMPLE4_STOP,1);
			//outlet(SAMPLE5_STOP,1);
			//singleFishSample[1] = false;
		}else{
			
			fadingOutSample[0] = true;
			//outlet(SAMPLE2_STOP,1);
			//outlet(SAMPLE3_STOP,1);
			//singleFishSample[0] = false;
		}
		
	}
	
	private void updateSingleFishSample(int fishNum){
		
		System.out.println("updating single fish sample "+ fishNum);
		
		volume = 2;
		noteDensity = 2;
		phraseLength = 2;
	
		
		if(fishNum == 1){
			if(size1 > .8){
				register = 1;
				timbre = 1;
				chordality = 2;
			}else if(size1 <.8 && size1 >.35){
				timbre = 1;
				register =2;
				chordality = 1;
			}else{
				timbre = 2;
				register = 3;
				chordality = 1;
			}
		}else{
			if(size2 > .8){
				register = 1;
				timbre = 1;
				chordality = 2;
			}else if(size2 <.8 && size2 >.35){
				timbre = 1;
				register =2;
				chordality = 1;
			}else{
				timbre = 2;
				register = 3;
				chordality = 1;
			}
			
		}
					
		setSingleFishSample(fishNum);
	
	}
	
	private boolean singleFishSampleIsPlaying(int fishNum){
		
		
		if(fishNum == 1){
			if(singleFishSample[0]){
				return true;
			}else{
				return false;
			}
		}else{
			if(singleFishSample[1]){
				return true;
			}else{
				return false;
			}			
		}
	}
	
	private boolean withinSizeThreshold(int fishNum){
		
		int[] compareSize = new int[2];

		if(fishNum == 1){

			for(int i = 0; i < 2; i++){

				if(previousSize1[i] < .32f){
					compareSize[i] = 1;

				}else if(previousSize1[i] >= .32f && previousSize1[i] <.51){
					compareSize[i] = 2;

				}else if(previousSize1[i] >= .51 && previousSize1[i] < .61){
					compareSize[i] = 3;

				}else if(previousSize1[i] >= .61 && previousSize1[i] < .8){
					compareSize[i] = 4;

				}else if(previousSize1[i] >= .8){
					compareSize[i] = 5;
				}
			}

		}else{
			for(int i = 0; i < 2; i++){

				if(previousSize2[i] < .32f){
					compareSize[i] = 1;

				}else if(previousSize2[i] >= .32f && previousSize2[i] <.51){
					compareSize[i] = 2;

				}else if(previousSize2[i] >= .51 && previousSize2[i] < .61){
					compareSize[i] = 3;

				}else if(previousSize2[i] >= .61 && previousSize2[i] < .8){
					compareSize[i] = 4;

				}else if(previousSize2[i] >= .8){
					compareSize[i] = 5;
				}
			}
			
		}
		
		if(compareSize[0] != compareSize[1]){
			return false;
		}else{
			return true;
		}
		
	}
	
	private boolean withinPriorityThreshold(int fishNum){
		
		//System.out.println("priority - "+ fishNum);
		int[] comparePriority = new int[2];
		
		if (fishNum == 2){
			for(int i = 0; i < 2; i++){

				if(previousSize2[i] < 0){
					comparePriority[i] = 1;
				}else{
					comparePriority[i] = 2;
				}

			}

			if(comparePriority[0] != comparePriority[1]){
				return false;
			}else{
				return true;
			}
			
		}else{

			for(int i = 0; i < 2; i++){

				if(previousSize1[i] < 0){
					comparePriority[i] = 1;
				}else{
					comparePriority[i] = 2;
				}

			}

			if(comparePriority[0] != comparePriority[1]){
				return false;
			}else{
				return true;
			}
		}
		
	}
	
	private void setOverallSample(){
		//sampleExists = getSampleFromData(volume, phraseLength, register);
		
		if(sampleExists){
			outlet(0, selectedSample);
			overallSamplePlaying = true;
		}else{
			
			if(checkAltVolumeSetting() == true){
				outlet(0, selectedSample);
				overallSamplePlaying = true;
			}else{					
				generatePhrase();
			}
			
		}
	}
	
	
	private void setSingleFishSample( int fishNum){
		
		int outletNum;
		int fadeOutlet;
		if(fishNum == 1){
			outletNum = SAMPLE2;
			fadeOutlet = FADEVOLUMES[0][0];
		}else{
			outletNum = SAMPLE4;
			fadeOutlet = FADEVOLUMES[1][0];
		}
		//test alternate volumes, phraseLength, then register
		
		sampleExists = getSampleFromData(volume, phraseLength, register,fishNum);
		System.out.println(sampleExists + "  fishNum-" + fishNum);
		
		//System.out.println(selectedSample);
		if(sampleExists){
			fileLength[fishNum-1] = getFileInfo(selectedSample);
			fadeVolumes[fishNum-1][0] = 127;
			fadeVolumes[fishNum-1][1] = 50;
			outlet(fadeOutlet, 127);
			outlet(fadeOutlet+4, 50);
			initFadeParameters(fishNum-1);
			sampleStartTime[fishNum-1][0] = System.currentTimeMillis();
			repetitionCount[fishNum-1] = 2;
			fadingInSample[fishNum-1] = true;
			outlet(outletNum, selectedSample);  //set samples files into 2 buffers so they can loop and crossfade into each other
			outlet(outletNum+4, selectedSample);
			outlet(outletNum+5,0);  //make sure to stop the second sample from playing 
			singleFishSample[fishNum - 1] = true;
			stopGeneratingMelody();
			
		}else{
			/*
			if(checkAltVolumeSetting() == true){
				fileLength[fishNum-1] = getFileInfo(selectedSample);
				fadingInSample[fishNum-1] = true;
				fadeVolumes[fishNum-1][0] = 127;
				fadeVolumes[fishNum-1][1] = 50;
				outlet(fadeOutlet, 127);
				outlet(fadeOutlet+4, 50);
				initFadeParameters(fishNum-1);
				sampleStartTime[fishNum-1][0] = System.currentTimeMillis();
				repetitionCount[fishNum-1] = 2;
				outlet(outletNum, selectedSample);
				outlet(outletNum+4, selectedSample);
				outlet(outletNum+5,0);  //make sure to stop the second sample from playing 
				singleFishSample[fishNum - 1] = true;
				stopGeneratingMelody();
				
			}else{					
				if(checkAltPhraseLength() == true){
					fileLength[fishNum-1] = getFileInfo(selectedSample);
					fadingInSample[fishNum-1] = true;
					fadeVolumes[fishNum-1][0] = 127;
					fadeVolumes[fishNum-1][1] = 50;
					outlet(fadeOutlet, 127);
					outlet(fadeOutlet+4, 50);
					initFadeParameters(fishNum-1);
					sampleStartTime[fishNum-1][0] = System.currentTimeMillis();
					repetitionCount[fishNum-1] = 2;
					outlet(outletNum,selectedSample);
					outlet(outletNum+4, selectedSample);
					outlet(outletNum+5,0);  //make sure to stop the second sample from playing 
					singleFishSample[fishNum - 1] = true;
					stopGeneratingMelody();
					
				}else{
					generatePhrase();
					
				}
				
			}
			*/
		}
	}
	
	private void repeatSample(int outletNum){
		outlet(outletNum, currentPlayingSample);
		singleFishSample[outletNum - 1] = true;
	}
	
	private boolean getSampleFromData(int tempVolume, int tempPhraseLength, int tempRegister)
	{
		String sample = "samples";
		
		switch(noteDensity){
		case 0:
			sample = sample.concat("/Single Note");
			break;
		case 1:
			sample = sample.concat("/low note density");
			break;
		case 2:
			sample = sample.concat("/medium note density");
			break;
		case 3:
			sample = sample.concat("/high note density");
			break;
		default:
			sample = sample.concat("/ERROR IN NOTE DENSITY");
			break;
		}
		
		
		switch(tempPhraseLength){
		case 0:
			sample = sample.concat("/Short Phrase");
			break;
		case 1:
			sample = sample.concat("/Short Phrase");
			break;
		case 2:
			sample = sample.concat("/Medium Phrase");
			break;
		case 3:
			sample = sample.concat("/Long Phrase");
			break;
		default:
			sample = sample.concat("/ERROR IN PHRASE LENGTH");
			break;
		}
		
		
		switch(timbre){
		case 0:
			sample = sample.concat("/arco (bowed)");
			break;
		case 1:
			sample = sample.concat("/arco (bowed)");
			break;
		case 2:
			sample = sample.concat("/pizzicato (plucked)");
			break;
		default:
			sample = sample.concat("/ERROR IN TIMBRE");
			break;
		}
		
		
		switch(chordality){
		case 0:
			sample = sample.concat("/singleNote");
			break;
		case 1:
			sample = sample.concat("/singleNote");
			break;
		case 2:
			sample = sample.concat("/chord");
			break;
		default:
			sample = sample.concat("/ERROR IN CHORDALITY");
			break;
		}
		
		switch(tempRegister){
		case 0:
			sample = sample.concat("/low register");
			break;
		case 1:
			sample = sample.concat("/low register");
			break;
		case 2:
			sample = sample.concat("/medium register");
			break;
		case 3:
			sample = sample.concat("/high register");
			break;
		default:
			sample = sample.concat("/ERROR IN REGISTER");
			break;
		}
		
		switch(tempVolume){
		case 0:
			sample = sample.concat("/low volume/");
			break;
		case 1:
			sample = sample.concat("/low volume/");
			break;
		case 2:
			sample = sample.concat("/medium volume/");
			break;
		case 3:
			sample = sample.concat("/high volume/");
			break;
		default:
			sample = sample.concat("/ERROR IN VOLUME");
			break;
		}
		

		sample = homeDirectory.concat(sample);
		//System.out.println(sample);
		File dir = new File(sample);
		String[] fileNames = dir.list();
		if (fileNames == null) {
		    // Either dir does not exist or is not a directory
			System.out.println("dir not exist!1");
			return false;
		} else {
			if(fileNames.length == 0){
				return false;
			}else{
				for (int i=0; i<fileNames.length; i++) {
					// Get filename of file or directory
					String filename = fileNames[i];
					//System.out.println(filename);
				}
			}
		}
		
		

		// It is also possible to filter the list of returned files.
		// This example does not return any files that start with `.'.
		FilenameFilter filter = new FilenameFilter() {
		    public boolean accept(File dir, String name) {
		        return !name.startsWith(".");
		    }
		};
		fileNames = dir.list(filter);
		
		int length = fileNames.length;
		int sampleNum;
		if (length > 0){
			Random randomValue = new Random();
			 sampleNum = randomValue.nextInt(length);
			 //sample = sample.concat("/");
			 sample = sample.concat(fileNames[sampleNum]);
		}
		selectedSample = sample;
		currentPlayingSample = selectedSample;
		return true;
	}
	
	
	
	private boolean getSampleFromData(int tempVolume, int tempPhraseLength, int tempRegister, int fishNum)
	{
		
		String sample = "";
		if(fishNum == 1){
			sample = "samplesNew/" + Integer.toString(fish1SizeLevel) + "/";
		}
		else{
			sample = "samplesNew/" + Integer.toString(fish2SizeLevel) + "/";
		}
			
		
		sample = homeDirectory.concat(sample);
		//sample = "/".concat(sample);
		System.out.println(sample);
		File dir = new File(sample);
		String[] fileNames = dir.list();
		if (fileNames == null) {
		    // Either dir does not exist or is not a directory
			System.out.println("dir not exist!2");
			return false;
		} else {
			if(fileNames.length == 0){
				return false;
			}else{
				for (int i=0; i<fileNames.length; i++) {
					// Get filename of file or directory
					String filename = fileNames[i];
					//System.out.println(filename);
				}
			}
		}
		
		

		// It is also possible to filter the list of returned files.
		// This example does not return any files that start with `.'.
		FilenameFilter filter = new FilenameFilter() {
		    public boolean accept(File dir, String name) {
		        return !name.startsWith(".");
		    }
		};
		fileNames = dir.list(filter);
		
		int length = fileNames.length;
		int sampleNum;
		if (length > 0){
			Random randomValue = new Random();
			 sampleNum = randomValue.nextInt(length);
			 //sample = sample.concat("/");
			 sample = sample.concat(fileNames[sampleNum]);
		}
		selectedSample = sample;
		currentPlayingSample = selectedSample;
		return true;
	}
	
	public void setHomeDirectory(String s){
	/*	String[] array;
		array = s.split(":");
		if(array.length > 1){
		homeDirectory = array[1];
		}else{
			homeDirectory = s;
		}*/
		if(s.contains("HD:")){
			homeDirectory = s.substring(3);
		}else{
			homeDirectory = s;
		}
		System.out.println("directory = "+homeDirectory);
		
	}
	
	
	private long getFileInfo(String audiofile){
		try {
			// From file
			AudioInputStream stream = AudioSystem.getAudioInputStream(new File(audiofile));


			// At present, ALAW and ULAW encodings must be converted
			// to PCM_SIGNED before it can be played
			AudioFormat format = stream.getFormat();
			if (format.getEncoding() != AudioFormat.Encoding.PCM_SIGNED) {
			format = new AudioFormat(
			AudioFormat.Encoding.PCM_SIGNED,
			format.getSampleRate(),
			format.getSampleSizeInBits()*2,
			format.getChannels(),
			format.getFrameSize()*2,
			format.getFrameRate(),
			true); // big endian
			stream = AudioSystem.getAudioInputStream(format, stream);
			}

			long length = (long)(stream.getFrameLength() / 44100.f) *1000;
			
			return length;
			
		}catch (Exception e) {
            e.printStackTrace();
            return -1;
		}

	}
	
	private void initFadeParameters(int fishNum){
		interFadeOut[fishNum][0] = false;
		interFadeOut[fishNum][1] = false;
		sampleInitiated[fishNum][0] = false;
		sampleInitiated[fishNum][1] = false;
		
	}
	
	private void adjustInterFades(){
		long currentTime;
		for(int i=0; i <2; i++){
			currentTime = System.currentTimeMillis();
			if(singleFishSample[i] == true){
				if(repetitionCount[i]%2 ==0){
					//System.out.println("samples 1 and 3");
					
					if(fadeVolumes[i][0] < 127){
						fadeVolumes[i][0] = fadeVolumes[i][0] +1.f;
						outlet(FADEVOLUMES[i][0], fadeVolumes[i][0]);
					}
					
					if(interFadeOut[i][1] == true){
						fadeVolumes[i][1] = fadeVolumes[i][1] -.8f;
						outlet(FADEVOLUMES[i][1], fadeVolumes[i][1]);
						if(fadeVolumes[i][1] <= 50){
							interFadeOut[i][1] = false;
						}
					}
					if(currentTime - sampleStartTime[i][0] > .7*fileLength[i] && currentTime - sampleStartTime[i][0] < .8*fileLength[i]){
						if(sampleInitiated[i][1] == false){
							if(i == 0){
								outlet(SAMPLE3_START, 1);
							}else{
								outlet(SAMPLE5_START,1);
							}
							sampleInitiated[i][1] = true;
							sampleStartTime[i][1] = currentTime;
						}
						if(fadeVolumes[i][1] < 127){
							fadeVolumes[i][1] = fadeVolumes[i][1] +.8f;
							outlet(FADEVOLUMES[i][1], fadeVolumes[i][1]);
						}
					}else if(currentTime - sampleStartTime[i][0] > .8*fileLength[i]){
						repetitionCount[i]++;
						interFadeOut[i][0] = true;
						sampleInitiated[i][0] = false;
					}

				}else{
					//System.out.println("samples 2 and 4");
					if(fadeVolumes[i][1] < 127){
						fadeVolumes[i][1] = fadeVolumes[i][1] +1.f;
						outlet(FADEVOLUMES[i][1], fadeVolumes[i][1]);
					}
					
					if(interFadeOut[i][0] == true){
						fadeVolumes[i][0] = fadeVolumes[i][0] -.8f;
						outlet(FADEVOLUMES[i][0], fadeVolumes[i][0]);
						if(fadeVolumes[i][0] <= 50){
							interFadeOut[i][0] = false;
						}
					}

					if(currentTime - sampleStartTime[i][1] > .7*fileLength[i] && currentTime - sampleStartTime[i][1] < .8*fileLength[i]){
						if(sampleInitiated[i][0] == false){
							if(i ==0){
								outlet(SAMPLE2_START, 1);
							}else{
								outlet(SAMPLE4_START, 1);
							}
							sampleInitiated[i][0] = true;
							sampleStartTime[i][0] = currentTime;
							interFadeOut[i][0] = false;
						}
						if(fadeVolumes[i][0] < 127){
							fadeVolumes[i][0] = fadeVolumes[i][0] +.8f;
							outlet(FADEVOLUMES[i][0], fadeVolumes[i][0]);
						}
					}else if(currentTime - sampleStartTime[i][1] > .8*fileLength[i]){
						repetitionCount[i]++;
						interFadeOut[i][1] = true;
						sampleInitiated[i][1] = false;
					}

				}
			}
		}
		
	}
	
	private void fadeOut(int sampleNum){

		//fadeVolumes[sampleNum][0] = fadeVolumes[sampleNum][0] - .3f;
		//fadeVolumes[sampleNum][1] = fadeVolumes[sampleNum][1] - .3f;
		sampleVolume[sampleNum] = sampleVolume[sampleNum] - .7f;
		if(sampleVolume[sampleNum] < 50){
			if(sampleNum == 0){
				outlet(SAMPLE2_STOP,0);
				outlet(SAMPLE3_STOP,0);
			}else{
				outlet(SAMPLE4_STOP,0);
				outlet(SAMPLE5_STOP,0);
			}
			singleFishSample[sampleNum] = false;
			fadingOutSample[sampleNum] = false;
		}

	}
	
	private void fadeIn(int sampleNum, int targetVolume){

		//fadeVolumes[sampleNum][0] = fadeVolumes[sampleNum][0] + .3f;
		//fadeVolumes[sampleNum][1] = fadeVolumes[sampleNum][1] + .3f;
		sampleVolume[sampleNum] = sampleVolume[sampleNum] + .9f;
		if(sampleVolume[sampleNum] >= targetVolume){
			sampleVolume[sampleNum] = targetVolume;
			fadingInSample[sampleNum] = false;
		}

	}
	
	private boolean fileStillPlaying(int sampleNum){
		
		long currentTime = System.currentTimeMillis();
		if(currentTime - sampleStartTime[sampleNum][0] >= fileLength[sampleNum]){
			return false;
		}else{
			return true;
		}
		
	}
	
	
	private boolean checkAltVolumeSetting(){
		
		for(int i = 0; i <3; i++){
			if(i != volume){
				if(getSampleFromData(i, phraseLength, register) == true){
					return true;
				}
			}
		}
		
		return false;
	}
	
	private boolean checkAltPhraseLength(){
		
		//checking phrase length with set volume
		for(int i = 0; i <3; i++){
			if(i != phraseLength){
				if(getSampleFromData(volume, i, register) == true){
					return true;
				}else{
					//checking phrase length with alt volume
					for(int j = 0; j <3; j++){
						if(j != volume){
							if(getSampleFromData(j, i, register) == true){
								return true;
							}
						}
					}
				}
			}
		}
		
		return false;
	}
	
	private void generatePhrase(){
		
		setNoteFile();
		setNoteDensity();
		
		if(generatingMelody[0] == false){
			outlet(6,1);
		}
		generatingMelody[0] = true;
		System.out.println("generating melody" +" - " + noteDensity + " - " +phraseLength + " - "+timbre +" - " + chordality  + " - " + register + " - "+ volume);
		System.out.println(selectedSample);	
	}
	
	private void stopGeneratingMelody(){
		//outlet(MELODY_OUTLET,0);
		generatingMelody[0] = false;
	}
	
	
	private void setNoteFile(){
		
		//note file based on size of fish
		if(size1 > .45){
			noteFileNum = 0;
		}else{
			noteFileNum = 6;
		}
		
	}
	
	private void setNoteDensity(){
		
		switch(noteFileNum){
		case 0:
			realTimeNoteDensity = degreeMovement*.6f;
			break;
		case 6:
			realTimeNoteDensity = degreeMovement*.8f;
			break;
		}
		
	}
	
	private void setSpatializationAndVolume(){
		
		//set the spatialization coordinates according to the fish position
		//set the volume coordinates according to fish position so that it is loudest when in the middle of x-axis
		
		float tempParams[] = new float[2];
		int tempVol[] = new int[2];
		
		tempParams[0] = (float)((fish1X - 0.5) * X_SCREEN_SIZE);
		tempParams[1] = (float)( (Y_SCREEN_SIZE) * (fish1Y - 0.5));
		tempVol[0] = (int)(127 - (Math.abs(.5f - fish1X)  * 40));
		outlet(SPATIALIZE1, tempParams);
		//outlet(VOLUME1, tempVol);
		
		
		tempParams[0] = (float)((fish2X - 0.5) * X_SCREEN_SIZE);
		tempParams[1] = (float)( (Y_SCREEN_SIZE) * (fish2Y - 0.5));
		tempVol[1] = (int)(127 - (Math.abs(.5f - fish2X)  * 40));
		outlet(SPATIALIZE2, tempParams);
		//outlet(VOLUME2, tempVol);
		
		
		setFades(tempVol);
		
	}
	
	private void setFades(int[] volumes){
		
		for(int i =0; i < 2;i++){
			if(singleFishSample[i] == true){
				if(fadingOutSample[i] == true){
					fadeOut(i);
				}else if(fadingInSample[i] == true){
					fadeIn(i, volumes[i]);
				}else{

				}
			}
		}
		outlet(VOLUME1, sampleVolume[0]);
		outlet(VOLUME2, sampleVolume[1]);
	}
	
	private int verifyNote(int note, int fileNum){
		int tempDivisor;
		switch(fileNum){
		case 0:
			if(note > 102){
			tempDivisor = (note - 102)/12;
			note = note - (12 * (tempDivisor+1));
		}else if(note < 78){
			tempDivisor = (78 - note)/12;
			note = note + (12 * (tempDivisor+1));
		}
			break;
		case 6:
			if(note > 88){
				tempDivisor = (note - 88)/12;
				note = note - (12 * (tempDivisor+1));
			}else if(note < 64){
				tempDivisor = (64 - note)/12;
				note = note + (12 * (tempDivisor+1));
			}
			break;
		}
		
		return(note);
	}
	
	public void sampleFinished(int sample ){
		
		switch(sample){
		case 1:
			overallSamplePlaying = false;
			break;
		case 2:
			singleFishSample[0] = false;
			break;
		case 3:
			singleFishSample[1] = false;
			break;
			
		}
		
		
	}
	
	
	private void setLaurieDrumTempo(){
		
		outlet(DRUM_TEMPO, 360);
		
	}
	
	
	private void setDrumTempo(){
		//tempo can be between 190 and 110 millisecond intervals
		
		int sizeLevel;
		
		if(fish1SizeLevel>0 || fish2SizeLevel >0){
			if(fish1SizeLevel > 0 && fish2SizeLevel > 0){
				sizeLevel =(int)((fish1SizeLevel + fish2SizeLevel) / 2.0f);
			}else if(fish1SizeLevel > 0 && fish2SizeLevel <= 0){
				sizeLevel = fish1SizeLevel;
			}else{
				sizeLevel = fish2SizeLevel;
			}
		}else{
			sizeLevel = 0;
		}
		
		switch(sizeLevel){
		case 0:
			drumTempo = (int)(190 - (excitement * 100));
			outlet(DRUM_TEMPO, drumTempo);
			break;
		case 1:
			drumTempo = (int)(190 - (excitement * 100)) - ((5-sizeLevel)*10) ;
			outlet(DRUM_TEMPO, drumTempo);
			break;
		case 2:
			drumTempo = (int)(190 - (excitement * 100)) - ((5-sizeLevel)*10) ;
			outlet(DRUM_TEMPO, drumTempo);
			break;
		case 3:
			drumTempo = (int)(190 - (excitement * 100)) - ((5-sizeLevel)*10) ;
			outlet(DRUM_TEMPO, drumTempo);
			break;
		case 4:
			drumTempo = (int)(190 - (excitement * 100)) - ((5-sizeLevel)*10) ;
			outlet(DRUM_TEMPO, drumTempo);
			break;
		case 5:
			drumTempo = (int)(190 - (excitement * 100)) - ((5-sizeLevel)*10) ;
			outlet(DRUM_TEMPO, drumTempo);
			break;
		}
		
		
		//drumTempo = (int)(190 - (excitement * 100));
		//outlet(DRUM_TEMPO, drumTempo);
		
	}
	
	
	private void createDrumPattern(){
		
		//excitement controls overall volume, tempo
		//density controls accents and note density
		//size controls specific drums
		//position controls tempo
		
		float tempBass[] = new float[16];
		float tempSnare[] = new float[16];
		float tempTom[] = new float[16];
		float tempBongo1[] = new float[16];
		float tempBongo2[] = new float[16];
		float tempCrash[] = new float[16];
		float tempRide1[] = new float[16];
		float tempRide2[] = new float[16];
		float tempFinger[] = new float[16];
		float tempChina[] = new float[16];
		
		int sizeLevel;
		
		if(fish1SizeLevel>0 || fish2SizeLevel >0){
			if(fish1SizeLevel > 0 && fish2SizeLevel > 0){
				sizeLevel =(int)((fish1SizeLevel + fish2SizeLevel) / 2.0f);
			}else if(fish1SizeLevel > 0 && fish2SizeLevel <= 0){
				sizeLevel = fish1SizeLevel;
			}else{
				sizeLevel = fish2SizeLevel;
			}
			
			switch(sizeLevel){
			case 1:
				for(int i = 0; i < 16; i++){
					tempBass[i] = BASSDRUM[i] - 1.f;
					tempSnare[i] = SNARE[i] - 1.f;
					tempTom[i] = FLOORTOM[i] - 1.f;
					tempBongo1[i] = BONGO1[i];
					tempBongo2[i] = BONGO2[i];
					/*tempRide1[i] = .3f;
					tempFinger[i] = .15f;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;*/
					
					tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;
				}
				break;
			case 2:
				for(int i = 0; i < 16; i++){
					tempBass[i] = BASSDRUM[i] - 1.f;
					tempSnare[i] = SNARE[i] - 1.f;
					tempTom[i] = FLOORTOM[i] - .8f;
					tempBongo1[i] = BONGO1[i] - .2f;
					tempBongo2[i] = BONGO2[i] - .2f;
					/*tempRide1[i] = .15f;
					tempFinger[i] = .1f;
					tempCrash[i] = .05f;
					tempRide2[i] = 0;
					tempChina[i] = .1f;*/
					
					tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;
				}
				break;
			case 3:
				for(int i = 0; i < 16; i++){
					tempBass[i] = BASSDRUM[i] - .8f;
					tempSnare[i] = SNARE[i] - .8f;
					tempTom[i] = FLOORTOM[i] - .6f;
					tempBongo1[i] = BONGO1[i] - .4f;
					tempBongo2[i] = BONGO2[i] - .4f;
					/*tempRide1[i] = .1f;
					tempFinger[i] = .1f;
					tempCrash[i] = .1f;
					tempRide2[i] = 0;
					tempChina[i] = .1f;*/
					
					tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;
				}
				break;
			case 4:
				for(int i = 0; i < 16; i++){
					tempBass[i] = BASSDRUM[i] - .3f;
					tempSnare[i] = SNARE[i] - .5f;
					tempTom[i] = FLOORTOM[i] - .2f;
					tempBongo1[i] = BONGO1[i] - .7f;
					tempBongo2[i] = BONGO2[i] - .7f;
					/*tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = .2f;
					tempRide2[i] = 0;
					tempChina[i] = .2f;*/
					
					tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;
				}
				break;
			case 5:
				for(int i = 0; i < 16; i++){
					tempBass[i] = BASSDRUM[i];
					tempSnare[i] = SNARE[i];
					tempTom[i] = FLOORTOM[i];
					tempBongo1[i] = BONGO1[i] - 1.f;
					tempBongo2[i] = BONGO2[i] - 1.f;
					/*tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = .5f;
					tempRide2[i] = 0;
					tempChina[i] = .25f;*/
					tempRide1[i] = 0;
					tempFinger[i] = 0;
					tempCrash[i] = 0;
					tempRide2[i] = 0;
					tempChina[i] = 0;
				}
				break;
			default:
				break;
			}
					
		}else{
			
			for(int i = 0; i < 16; i++){
				tempBass[i] = 0;
				tempSnare[i] = 0;
				tempTom[i] = .05f;
				tempBongo1[i] = .1f;
				tempBongo2[i] = .1f;
				tempRide1[i] = .05f;
				tempFinger[i] = .05f;
				/*tempCrash[i] = 0;
				tempRide2[i] = 0;
				tempChina[i] = 0;*/
				
				tempRide1[i] = 0;
				tempFinger[i] = 0;
				tempCrash[i] = 0;
				tempRide2[i] = 0;
				tempChina[i] = 0;
			}

		}
		
		for(int i=0; i <16;i++){
			drumPattern[0][i][0] = shouldPlayNote(tempBass[i]) ? 1 : 0;
			drumPattern[1][i][0] = shouldPlayNote(tempSnare[i]) ? 1 : 0;
			drumPattern[2][i][0] = shouldPlayNote(tempBongo1[i]) ? 1 : 0;
			drumPattern[3][i][0] = shouldPlayNote(tempBongo2[i]) ? 1 : 0;
			drumPattern[4][i][0] = shouldPlayNote(tempTom[i]) ? 1 : 0;
			drumPattern[5][i][0] = shouldPlayNote(tempRide1[i]) ? 1 : 0;
			drumPattern[6][i][0] = shouldPlayNote(tempRide2[i]) ? 1 : 0;
			drumPattern[7][i][0] = shouldPlayNote(tempCrash[i]) ? 1 : 0;
			drumPattern[8][i][0] = shouldPlayNote(tempChina[i]) ? 1 : 0;
			drumPattern[9][i][0] = shouldPlayNote(tempFinger[i]) ? 1 : 0;	
		}
		
		
		//set up volumes for accenting effects
		for(int i=0; i <16;i++){
			drumPattern[0][i][1] = DRUMVOLUMES[0];
			drumPattern[1][i][1] = DRUMVOLUMES[1];
			drumPattern[2][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[2] : DRUMVOLUMES[2] - 6;
			drumPattern[3][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[3] : DRUMVOLUMES[3] - 6;
			drumPattern[4][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[4] : DRUMVOLUMES[4] - 6;
			drumPattern[5][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[5] : DRUMVOLUMES[5] - 6;
			drumPattern[6][i][1] = DRUMVOLUMES[6];
			drumPattern[7][i][1] = DRUMVOLUMES[7];
			drumPattern[8][i][1] = DRUMVOLUMES[8];
			drumPattern[9][i][1] = DRUMVOLUMES[9];	
		}
		
		
		
		hitNum = 0;  //reset drum hit counter and then play the first beat
		int hits[] = new int[16];
		int accents[] = new int[16];
		for(int i =0; i < 10; i++){
			accents[i] = drumPattern[i][hitNum][1];
			hits[i] = drumPattern[i][hitNum][0];
		}		
		hitNum++;
		outlet(DRUM_VOLUME, accents);
		outlet(DRUM_HITS, hits);
	}
	
	private void setLaurieDrum(){
		
		if(fish1SizeLevel == 5 || fish2SizeLevel == 5){
			if(laurieDrumPlaying == false){
				createLaurieDrumPattern();
				setLaurieDrumTempo();
				laurieDrumPlaying = true;
			}
			setLaurieDrumVolume();
		}else{
			laurieDrumPlaying = false;
		}
		
	}
	
	private void setLaurieDrumVolume(){
		
		float outputs[] = new float[3];
		if(fish1SizeLevel == 5 && fish2SizeLevel!=5){
			
			outputs[0] = 0;
			outputs[1] = 1;			
			outputs[2] = (int)(93 - (Math.abs(.5f - fish1X)  * 30));
			outlet(SPATIALIZE_DRUM, outputs);
			
		}else if(fish2SizeLevel == 5 && fish1SizeLevel!=5){		
			
			outputs[0] = 0;
			outputs[1] = 1;			
			outputs[2] = (int)(93 - (Math.abs(.5f - fish2X)  * 30));
			outlet(SPATIALIZE_DRUM, outputs);
			
		}else if(fish1SizeLevel == 5 && fish2SizeLevel == 5){
	
			outputs[0] = 0;
			outputs[0] = 1;
			int temp1 = (int)(93 - (Math.abs(.5f - fish1X)  * 30));
			int temp2 = (int)(93 - (Math.abs(.5f - fish2X)  * 30));
			if(temp1 > temp2){
				outputs[2] = temp1;
			}else{
				outputs[2] = temp2;
			}
		}
		
	}
	
	private void createLaurieDrumPattern(){
		
		
		for(int i=0; i <16;i++){
			drumPattern[0][i][0] = shouldPlayNote(LABASSDRUM[i]) ? 1 : 0;
			drumPattern[1][i][0] = shouldPlayNote(LASNARE[i]) ? 1 : 0;
			drumPattern[3][i][0] = 0;
			drumPattern[4][i][0] = shouldPlayNote(LAFLOORTOM[i]) ? 1 : 0;
			drumPattern[5][i][0] = 0;
			drumPattern[6][i][0] = 0;
			drumPattern[7][i][0] = 0;
			drumPattern[8][i][0] = 0;
			drumPattern[9][i][0] = 0;
		}
		//set up volumes for accenting effects
		for(int i=0; i <16;i++){
			drumPattern[0][i][1] = DRUMVOLUMES[0];
			drumPattern[1][i][1] = DRUMVOLUMES[1];
			drumPattern[2][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[2] : DRUMVOLUMES[2] - 6;
			drumPattern[3][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[3] : DRUMVOLUMES[3] - 6;
			drumPattern[4][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[4] : DRUMVOLUMES[4] - 6;
			drumPattern[5][i][1] = shouldPlayNote(.4) ? DRUMVOLUMES[5] : DRUMVOLUMES[5] - 6;
			drumPattern[6][i][1] = DRUMVOLUMES[6];
			drumPattern[7][i][1] = DRUMVOLUMES[7];
			drumPattern[8][i][1] = DRUMVOLUMES[8];
			drumPattern[9][i][1] = DRUMVOLUMES[9];	
		}
		
		hitNum = 0;
		int hits[] = new int[16];
		int accents[] = new int[16];
		for(int i =0; i < 10; i++){
			accents[i] = drumPattern[i][hitNum][1];
			hits[i] = drumPattern[i][hitNum][0];
		}
		hitNum++;
		outlet(DRUM_VOLUME, accents);
		outlet(DRUM_HITS, hits);
	}
	
	private void setDrumVolandSpace(){
		
		float[] outputs = new float[3];
		float tempX;
		float tempY;
		float tempX2;
		float tempY2;
		
		//spatializations, in center for now
		outputs[0] = 0;
		outputs[1] = 0;
		
		
		/*if(fish1SizeLevel > 0 && fish2SizeLevel > 0){
			tempX = (float)((fish1X - 0.5) * X_SCREEN_SIZE);
			tempY = (float)( (Y_SCREEN_SIZE) * (fish1Y - 0.5));	
			
			tempX2 = (float)((fish2X - 0.5) * X_SCREEN_SIZE);
			tempY2 = (float)( (Y_SCREEN_SIZE) * (fish2Y - 0.5));
			
			outputs[0] = (tempX + tempX2)/2.f;
			outputs[1] = (tempX2 + tempY2)/2.f;
			
		}else if(fish1SizeLevel < 0 && fish2SizeLevel > 0){
			
			outputs[0] = (float)((fish2X - 0.5) * X_SCREEN_SIZE);
			outputs[1] = (float)( (Y_SCREEN_SIZE) * (fish2Y - 0.5));
			
		}else{			
			outputs[0] = (float)((fish1X - 0.5) * X_SCREEN_SIZE);
			outputs[1] = (float)( (Y_SCREEN_SIZE) * (fish1Y - 0.5));				
		}*/
		
		
		
		//volume
		outputs[2] = 73 + excitement*30;
		
		outlet(SPATIALIZE_DRUM, outputs);
		
	}
	
	
	//********* Markov Functions	***************
	public void resetAll()
	{
		for(int x = 0; x <totalMelodies; x++){
			for(int i=0; i<totalNotes[x];i++){
				for(int j =0;j<2;j++){
					notes[x][i][j] = 0;
				}
			}
		
			totalNotes[x] = 0;
			firstOrderLowerLimit[x] = 0;
			
		}
		resetMatrices();
	}
	
	public void resetIndividual(int melody){
		for(int i=0; i<totalNotes[melody];i++){
			for(int j =0;j<2;j++){
				notes[melody][i][j] = 0;
			}
		}
	
	totalNotes[melody] = 0;
	firstOrderLowerLimit[melody] = 0;
	resetIndividualMatrix(melody);
	}
	
	private void resetIndividualMatrix(int melody){
		for(int i = 0;i<totalNoteOptions;i++){
			totalOccurrences[melody][i] = 0;
			for(int j =0; j<totalNoteOptions;j++){
				totalNoteTransitions[melody][i][j] = 0;
				transitionNoteMatrix_1[melody][i][j] = 0;
				totalDurationTransitionOccurrences[melody][i][j] = 0;
				for(int y =0;y<totalDurationOptions;y++){
					totalDurationTypeTransitionOccurrences[melody][i][j][y] = 0;
					transitionDurMatrix_1[melody][i][j][y] = 0;
				}				
			}
		}
	}
	
	private void resetMatrices()
	{
		
		for(int x =0; x < totalMelodies; x++){
			for(int i = 0;i<totalNoteOptions;i++){
				totalOccurrences[x][i] = 0;
				for(int j =0; j<totalNoteOptions;j++){
					totalNoteTransitions[x][i][j] = 0;
					transitionNoteMatrix_1[x][i][j] = 0;
					totalDurationTransitionOccurrences[x][i][j] = 0;
					for(int y =0;y<totalDurationOptions;y++){
						totalDurationTypeTransitionOccurrences[x][i][j][y] = 0;
						transitionDurMatrix_1[x][i][j][y] = 0;
					}				
				}
			}
		}
	}
	
	
	private void updateTransitionMatrices(int melody)
	{
		int tempNote;
		int tempNextNote;
		int tempNextNextNote;
		for (int i = firstOrderLowerLimit[melody];i<totalNotes[melody]-1;i++){
			
			//setting note data (midi note)
			tempNote = notes[melody][i][0];
			tempNextNote = notes[melody][i+1][0];
			totalOccurrences[melody][tempNote]++; //= totalOccurences[notes[i][0]] + 1;
			totalNoteTransitions[melody][tempNote][tempNextNote]++; //= totalNoteTransitions[notes[i][0]][notes[i+1][0]] + 1;
			transitionNoteMatrix_1[melody][tempNote][tempNextNote] = (float)totalNoteTransitions[melody][tempNote][tempNextNote]/totalOccurrences[melody][tempNote];
			
			if(transitionNoteMatrix_1[melody][tempNote][tempNextNote] != 0.0){  //adjust probabilities if another is added
				for(int j=0; j<totalNoteOptions;j++){
					transitionNoteMatrix_1[melody][tempNote][j] = (float)totalNoteTransitions[melody][tempNote][j]/totalOccurrences[melody][tempNote];
				}
			}
		}
			
		
		
		//setting note length data (midi duration)
			for(int i = firstOrderLowerLimit[melody];i <totalNotes[melody]-1;i++){
				tempNote = notes[melody][i][0];
				tempNextNote = notes[melody][i+1][0];
				totalDurationTransitionOccurrences[melody][tempNote][tempNextNote]++;
				
				for(int j=0; j<totalDurationOptions;j++){
					if(notes[melody][i][1] == (int)possibleNoteDurations[j]){
						totalDurationTypeTransitionOccurrences[melody][tempNote][tempNextNote][j]++;
					}
					
					transitionDurMatrix_1[melody][tempNote][tempNextNote][j] = (float)totalDurationTypeTransitionOccurrences[melody][tempNote][tempNextNote][j]/totalDurationTransitionOccurrences[melody][tempNote][tempNextNote];
					if(transitionDurMatrix_1[melody][tempNote][tempNextNote][j] != 0.0){
						for(int x=0; x<totalDurationOptions; x++){
							transitionDurMatrix_1[melody][tempNote][tempNextNote][x] = (float)totalDurationTypeTransitionOccurrences[melody][tempNote][tempNextNote][x]/totalDurationTransitionOccurrences[melody][tempNote][tempNextNote];
						}
					}
				}
			}
		
		//update counter so when continuous learning is enabled the for loops to calculate you all the way from zero	
		firstOrderLowerLimit[melody] = totalNotes[melody];
		secondOrderLowerLimit[melody] = firstOrderLowerLimit[melody] - 1;
	}
	
	
	private int getNextFirstOrderNote(int currentNote, int melody)
	{
		
		int nextNote = 0;
		float sum = 0;
		double targetSum;
		
		Random randomValue = new Random();
		targetSum = ((double)randomValue.nextInt(10000)+1)/10000;
		
		
		//run 1st degree markov chain
		while((nextNote < totalNoteOptions) && (sum + transitionNoteMatrix_1[melody][currentNote][nextNote] <= targetSum)){
			sum += transitionNoteMatrix_1[melody][currentNote][nextNote];
			nextNote++;
		}
		
		if(nextNote == 0){
			System.out.println("GOT A ZERO " + sum + "  - " + targetSum);
		}
		
		//outlet(0,nextNote);
		//outlet(1,transitionNoteMatrix_1[60][62]);
		if( nextNote == 100){
			nextNote = notes[melody][0][0];
		}
		return nextNote;
		
	}
	
	private boolean shouldPlayNote(double probability){
		
		double temp;
		Random randomValue = new Random();
		temp = ((double)randomValue.nextInt(10000)+1)/10000;
		
		if (temp<= probability){
			return true;
		} else{
			return false;
		}
	}
	
	private boolean shouldPlayNote(float probability){
		
		double temp;
		Random randomValue = new Random();
		temp = ((double)randomValue.nextInt(10000)+1)/10000;
		
		if (temp<= probability){
			return true;
		} else{
			return false;
		}
	}
	
	
	public void analyzeData(){
		resetMatrices();
		updateTransitionMatrices(0);
		
		System.out.println("DATA ANALYSIS FINISHED");
	}
	
	
	public void loadNoteData(){
				
		//loading all of Laurie's transcriptions
		String path = "/Users/masonbretan/Desktop/School/Music Tech/Research/Sonification/Aquarium Project/markovTranscriptions/";
		File folder = new File(path);
	    File[] listOfFiles = folder.listFiles();
	    String[] files = folder.list();
		
	    //System.out.println("files -- " + listOfFiles.length +" -- " + files[0]);
	    
		
		//File aFile = new File("/Users/masonbretan/Desktop/test1.txt");
		String REGEX = " ";
		int counter = 0;
		
		String temp1;
		String temp2;
		//resize arrays
		notes = (int[][][])Arrays.copyOf(notes,listOfFiles.length);
		
	    
		//go through all the text files
		for(int melodyNum = 0; melodyNum < listOfFiles.length; melodyNum++){
			counter = 0;
			try {
				//use buffering, reading one line at a time
				//FileReader always assumes default encoding is OK!
				BufferedReader input =  new BufferedReader(new FileReader(path+files[melodyNum]));
				try {
					String line = null; //not declared within while loop
					/*
					 * readLine is a bit quirky :
					 * it returns the content of a line MINUS the newline.
					 * it returns null only for the END of the stream.
					 * it returns an empty String if two newlines appear in a row.
					 */
					while (( line = input.readLine()) != null){

						Pattern p = Pattern.compile(REGEX);
						String[] items = p.split(line);						

						//build note array for markov info
						//the 2nd and 3rd items in list are midi note and duration
						//System.out.println(melodyNum+"-" +items[2] +"-");
						temp1 = (items[1].trim()).replaceAll("\\D", "");
						temp2 = (items[2].trim()).replaceAll("\\D", "");
						notes[melodyNum][totalNotes[melodyNum]][0] = Integer.parseInt(temp1);
						notes[melodyNum][totalNotes[melodyNum]][1] = Integer.parseInt(temp2);
						totalNotes[melodyNum]++;
						
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
		
		resetMatrices();
		for(int i=0; i<listOfFiles.length; i++){
			updateTransitionMatrices(i);
		}
		
		System.out.println("musician data loaded!");
		
	}
    
}



