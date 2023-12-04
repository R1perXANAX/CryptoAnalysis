
public class Cracking {
    // frequency of each letter in the english alphabet, baseVector[0]=A, baseVector[1]=B etc.
    // i need 26 vectors like this shifted right one by one so I'll make a matrix of
    // wich each row represents a vector
    public static double baseVector[] = { 
            0.0812,
            0.0149,
            0.0271,
            0.0432,
            0.1202,
            0.0230,
            0.0203,
            0.0592,
            0.0731,
            0.001,
            0.0069,
            0.0398,
            0.0261,
            0.0695,
            0.0768,
            0.0182,
            0.0011,
            0.0602,
            0.0628,
            0.091,
            0.0288,
            0.0111,
            0.0209,
            0.0017,
            0.0211,
            0.007
    };
    

    // make a matrix using baseVector as rows
    public static double[][] FrequencyMatrix = new double[26][26];

    public static double[][] getFrequencyMatrix() {
        return FrequencyMatrix;
    }

    public static void makeFrequencyMatrix() {

        for (int j = 0; j < FrequencyMatrix[0].length; j++) {
            FrequencyMatrix[0][j] = baseVector[j];
        }
        for (int i = 1; i < FrequencyMatrix.length; i++) {
            for (int j = 0; j < FrequencyMatrix.length; j++) {
                if (j == 0)
                    FrequencyMatrix[i][0] = FrequencyMatrix[i - 1][(25)];
                else
                    FrequencyMatrix[i][j] = FrequencyMatrix[i - 1][(j - 1 % 26)];
            }

        }
    }

    public static int countLetter(char[] letters, char c) {
        int count = 0;
        for (char x : letters) {
            if (x == c){
                count++;
            }
        }
        return count;
    }

    public static double[] makeFrequencyVector(char[] text) {
        double[] vector = new double[26];
        
        int count;
        for (char app ='a';app<='z'; app++) {
            count=countLetter(text, app);

            vector[app-'a'] = count / 26.0;
        }
        return vector;

    }

    public static double product(double[] v1, double[] v2) {
        double n = 0;
        for (int i = 0; i < v1.length; i++) {
            n =n+ v1[i] * v2[i];
        }
        return n;
    }

    public static char[][] nthLetters(char[] ciphered, int keyLenght) {

        int numberOfColumns = (int) Math.ceil((double) ciphered.length / keyLenght);

        char[][] result = new char[keyLenght][numberOfColumns];

        int counter = 0;
        for (int i = 0; i < result[0].length; i++) {
            for (int j = 0; j < result.length; j++) {
                if (counter < ciphered.length) {
                    result[j][i] = ciphered[counter];
                    counter++;
                }
                else result[j][i]='/';

            }
        }
        return result;
    }

    public static int checkPermutation(double[] frequency) {
        int row = -1;
        double max = -1;
        double vectorialProduct;

        for (int i = 0; i < FrequencyMatrix.length; i++) {
            vectorialProduct = product(frequency, FrequencyMatrix[i]);

            if (vectorialProduct > max) {
                max = vectorialProduct;
                row = i;
            }
        }
        return row;
    }

    public static String crack(char[] ciphered, int keyLenght) {
        makeFrequencyMatrix();

        char[][] matrix = nthLetters(ciphered, keyLenght);
        double frequency[];
        int permutation;
        String key="";

        for (int i = 0; i < matrix.length; i++) {
            
            frequency = makeFrequencyVector(matrix[i]);
            permutation = checkPermutation(frequency);
            key=key+(char)('a'+permutation);
            
        }

        return key;
    }

}
