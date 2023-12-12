import java.util.ArrayList;
import java.util.List;

public class Executer implements Runnable{
    private int lengthToCheck;
    private String ciphered;

    public Executer(int lengthToCheck,String ciphered){
        this.lengthToCheck = lengthToCheck;
        this.ciphered = ciphered;
    }
    @Override
    public void run() {
        System.out.println("\n[+]Cracking with permutation: " + lengthToCheck);
        // --------------------------------- SINGLE COLUMN TRANSPOSITION ---------------------------------
        List<List<Integer>> permutations = new ArrayList<>();
        Utils.generatePermutationsForNumber(lengthToCheck, permutations); // creates all possible permutations of length lengthToCheck

        for (List<Integer> permutation : permutations){ // looping through each permutation
            String rearrangedText = Utils.rearrangeText(ciphered, permutation ); // rearrange the text according to the permutation

            // --------------------------------- CRACKING VIGENERE ---------------------------------
            String foundKey;
            String deciphered;

            // index of coincidence (= IC, a measure of how similar a frequency distribution is to the uniform distribution). 
            // The closer to the English IC (0.067, not normalized), the most likely the key is correct
            double IC = 0.0; 
            
            // check for key length 6 to 10, as the key is in this range
            for (int j = 6; j <= 10; j++){

                foundKey = Cracking.crack(rearrangedText.toCharArray(), j ); // I try to crack the key with the length j
                deciphered = Utils.decipher(rearrangedText, foundKey); // I decipher the text with the found key
                IC = Utils.calculateIC(deciphered);  // I calculate the IC of the deciphered text

                if(IC >= 0.06){
                    System.out.println("\n[+]COLUMN TRANSPOSITION KEY FOUND:" + permutation.toString());
                    System.out.println("\n[+]VIGENERE KEY FOUND:" +foundKey+", IC:" +IC);
                    System.out.println("[+]PLAIN TEXT: \n"+deciphered);
                }
            }
        }
        System.out.println("\n[-]Cracking with permutation: " + lengthToCheck + " finished");



    }
}
