from EnigmaMachine.enigma import Enigma, enigma_machine
from EnigmaMachine.plugboard import Plugboard
from EnigmaMachine.reflector import Reflector
from EnigmaMachine.rotor import Rotor

from Cracker import cracker

def main ():
   

    #ABCDEFGHIJKLMNOPQRSTUVWXYZ
    #EKMFLGDQVZNTOWYHXUSPAIBRCJ
    #AJDKSIRUXBLHWTMCQGZNPYFVOE
    #BDFHJLCPRTXVZNYEIWGAKMUSQO     
    rotor1 = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ",23,"I")
    rotor2 = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE",3,"II")
    rotor3 = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO",4,"III")
    reflector = Reflector("YRUHQSLDPXNGOKMIEBFZCWVJAT")
    #fast,medium,slow
    rotors = [rotor1,rotor2,rotor3]
    plugboard = Plugboard([])
    enigma_machine = Enigma(rotors,reflector,plugboard)
    
    message = "CIAOMAMMACOMESTAITUTTOBENESPERODISIBACIONI"
    encoded_message = enigma_machine.encode(message)
    print(encoded_message)

    rotor1_copy = Rotor("EKMFLGDQVZNTOWYHXUSPAIBRCJ", 23, "I")
    rotor2_copy = Rotor("AJDKSIRUXBLHWTMCQGZNPYFVOE", 3, "II")
    rotor3_copy = Rotor("BDFHJLCPRTXVZNYEIWGAKMUSQO", 4, "III")
    enigma_copy = Enigma([rotor1_copy,rotor2_copy,rotor3_copy],reflector,plugboard)
    decoded_message=enigma_copy.encode(encoded_message)
    print(decoded_message)

    
    cracker.create_gamma_k_graph(cracker.crib_analysis(cracker.create_crib_graph("CIAOMAMMACOMESTAI","QMXBVLSRDWVDUTQDMFJONJOSKNIBFTMFVEZSGDZHWM" )))
    
if __name__ == '__main__':
    main()