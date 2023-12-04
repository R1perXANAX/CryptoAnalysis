import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Utils {
	private static char [][]matrix;

	public static void createMatrix(){
		matrix=new char[26][26];
		int counter=0;
		for(int i=0;i<matrix.length;i++) {
			for(int j=0;j<matrix[0].length;j++) {
				//filling the row, eatch time i will start from the i_th letter
				if(('a'+i+j)<='z')
					//still haven't finished the alphabet
					matrix[i][j]=(char)('a'+i+j);
				else {
					//have to restart the alphabet
					matrix[i][j]=(char)('a'+counter);
					counter++;
				}
			}
			counter=0;			
		}
	}
	
	public static String decipher(String ciphered,String key) {
		String plain="";
		char []cipheredArray=ciphered.toCharArray();
		char []actualKey=makeKeySameLenghtPlain(ciphered, key);
		int indexCiphered=0;
		for(indexCiphered=0;indexCiphered<cipheredArray.length;indexCiphered++){
			for(int j=0;j<matrix.length;j++){
				if(matrix[j][ actualKey[indexCiphered]-'a' ]==cipheredArray[indexCiphered]){
					plain+= (char)('a'+j);
				}

			}				
		}
		return plain;	
	}
	
	public static char[] makeKeySameLenghtPlain(String plain,String key)  {
		if(key.length()>plain.length()) {
			key=key.substring(0,plain.length());
		}else {
			while(key.length()<plain.length()) {
				if(plain.length()-key.length()>key.length()) 
					key+=key;
				else
					key=key+key.substring(0,plain.length()-key.length());		
			}
		}
		
		return key.toCharArray();
		
		
	}

	private static void generatePermutations(List<Integer> list, int n, List<List<Integer>> allPermutations) {
        if (n == 1) {
            allPermutations.add(new ArrayList<>(list));
        } else {
            for (int i = 0; i < n - 1; i++) {
                generatePermutations(list, n - 1, allPermutations);
                if (n % 2 == 0) {
                    Collections.swap(list, i, n - 1);
                } else {
                    Collections.swap(list, 0, n - 1);
                }
            }
            generatePermutations(list, n - 1, allPermutations);
        }
    }

    public static void generatePermutationsForNumber(int n, List<List<Integer>> allPermutations) {
        List<Integer> list = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            list.add(i);
        }
        generatePermutations(list, n, allPermutations);
    }

    public static double calculateIC(String text){
		int[] letterFrequencies = new int[26];
        int totalLetters = 0;

        // Count the frequency of each letter (A-Z) in the text
        for (char c : text.toUpperCase().toCharArray()) {
            if (c >= 'A' && c <= 'Z') {
                letterFrequencies[c - 'A']++;
                totalLetters++;
            }
        }

        double ic = 0.0;

        // Calculate the Index of Coincidence
        for (int frequency : letterFrequencies) {
            ic += frequency * (frequency - 1);
        }
        ic /= (double) (totalLetters * (totalLetters - 1));

        return ic;
	}

	public static String rearrangeText(String cipheredText, List<Integer> permutation) {


		StringBuilder textCpy = new StringBuilder(cipheredText);

        int extraChars = textCpy.length() % permutation.size();
        int emptySpace = (extraChars == 0) ? 0 : (permutation.size() - extraChars);
        int columnSize = textCpy.length() / permutation.size();

		List<Integer> firstColumn = new ArrayList<>();
        List<Integer> lastColumn = new ArrayList<>();

        for (int i = 0; i < permutation.size(); i++) {
            if (i < permutation.size() - emptySpace)
                firstColumn.add(permutation.get(i));
            else
                lastColumn.add(permutation.get(i));
        }

		int i = 0, step, currPos = 0;

        while (currPos < textCpy.length()) {
            if (firstColumn.contains(i)) {
                step = columnSize + 1;
            } else {
                step = columnSize;
                textCpy.insert(currPos + step, ' ');
                currPos++;
            }
            currPos += step;
            i++;
        }

        // Creating the column map
        int extraChar = (emptySpace == 0) ? 0 : 1;
        List<String> columnMap = new ArrayList<>(permutation.size());
        for (int j = 0; j < permutation.size(); j++) {
            int perm_start = permutation.get(j);
            int start = perm_start * (columnSize + extraChar);
            columnMap.add(textCpy.substring(start, start + columnSize+extraChar));
        }

        // Reconstructing the result
        StringBuilder result = new StringBuilder();
        for (int x = 0; x < columnMap.get(0).length(); x++) {
            for (int y = 0; y < permutation.size(); y++) {
                char charToAdd = columnMap.get(y).charAt(x);
                if (charToAdd != ' ') {
                    result.append(charToAdd);
                }
            }
        }
        return result.toString();
    }
    

}
