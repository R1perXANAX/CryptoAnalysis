
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

    public static void makeFrequencyMatrix() { // create the matrix of frequency vectors. each row is a vector shifted right by one: row 0 is baseVector, row 1 is baseVector shifted right by one, row 2 is baseVector shifted right by two etc

        for (int j = 0; j < FrequencyMatrix[0].length; j++) { // first row is baseVector
            FrequencyMatrix[0][j] = baseVector[j];
        }
        for (int i = 1; i < FrequencyMatrix.length; i++) { // other rows are shifted right by one
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

    // make a vector of the frequency of each letter in the text 
    public static double[] makeFrequencyVector(char[] text) {
        double[] vector = new double[26];
        
        int count;
        for (char app ='a';app<='z'; app++) {
            count=countLetter(text, app);

            vector[app-'a'] = count / 26.0;
        }
        return vector;

    }
    // vectorial product of two vectors (of the same length)
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
    
    // check which permutation of the baseVector is the most similar to the frequency vector of the row

    // frequency .. .. .. .. --> frequency vector of the row I'm checking
    // c a o a --> frequency .. .. .. ..
    // i m m i     
    // a m e 
    // o a s 
    // m c t 
    // ---------------------------------
    // vectorial product:
    // -->  (.. .. .. ..) x (baseVector)
    // ---------------------------------

    // input: frequency vector of the row I'm checking
    public static int checkPermutation(double[] frequency) {
        int row = -1;
        double max = -1;           
        double vectorialProduct;        

        for (int i = 0; i < FrequencyMatrix.length; i++) { // for each row of the matrix
            vectorialProduct = product(frequency, FrequencyMatrix[i]); // vectorial product of the frequency vector of the row and the i_th row of the matrix

            if (vectorialProduct > max) {  // if the vectorial product is the biggest I've found so far
                max = vectorialProduct; // update the max
                row = i;
            }
        }
        return row; // output: index (n. row of the matrix)
    }
    // CRACKING VIGENERE CLASSIC
    public static String crack(char[] ciphered, int keyLenght) {
        makeFrequencyMatrix(); // create the matrix of frequency vectors

        // create a matrix that (among the other things) shows the letters that are in the same column.
        // That means that the letters in the same column are encrypted with the same letter of the key!!

        // example: key = "paolo", ciphered = "ciaomammacomestai"

        // c i a o m a m m a c o m e s t a i
        // p a o l o p a o l o p a o l o p a
        // ---------------------------------
        // p a o l o --> key length = 5

        // c i a o m --> 5 columns, 4 rows
        // m a m m a
        // c o m e s
        // t a i

        char[][] matrix = nthLetters(ciphered, keyLenght);

        // -------------output---------------
        // c a o a   --> 5 rows, 4 columns
        // i m m i
        // a m e 
        // o a s 
        // m c t
        // ---------------------------------

        double frequency[];
        int permutation;
        String key="";

        for (int i = 0; i < matrix.length; i++) { // for each row of the matrix

            // make a vector of the frequency of each letter in the row (simple n. of occurrences/26)
            frequency = makeFrequencyVector(matrix[i]); 
            // c a o a --> frequency .. .. .. ..
            // i m m i     
            // a m e 
            // o a s 
            // m c t 
            // ---------------------------------
            // frequency .. .. .. .. --> baseVector
            // ---------------------------------

            // check which permutation of the baseVector is the most similar to the frequency vector of the row
            permutation = checkPermutation(frequency); 

            // add the letter of the key that corresponds to the permutation (n. shift--> letter)
            key=key+(char)('a'+permutation);
            
        }

        return key;
    }

}
