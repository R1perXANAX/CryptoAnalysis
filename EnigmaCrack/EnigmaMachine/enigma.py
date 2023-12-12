from EnigmaMachine.plugboard import Plugboard
from EnigmaMachine.reflector import Reflector
from EnigmaMachine.rotor import Rotor
from typing import List

class Enigma:
    
    def __init__(self, rotors: "List[Rotor]", reflector:Reflector ,plugboard:Plugboard):
        self.rotors = rotors
        self.reflector = reflector
        self.plugboard = plugboard
        
    def encode(self, message):
        encoded_message = ""
        for letter in message:
            encoded_message += self.encode_letter(letter)
        return encoded_message
    
    def encode_letter(self, letter):
        self.rotate_rotors()
        letter = self.plugboard.encode(letter)
        for rotor in self.rotors:

            letter = rotor.encodeRotor(letter)

        letter = self.reflector.reflect(letter)

        for rotor in reversed(self.rotors):
            letter = rotor.reverseEncode(letter)

        letter = self.plugboard.encode(letter)
        return letter

    def rotate_rotors(self):
        self.rotors[0].rotate()
        for i in range(1, len(self.rotors)):
            if self.rotors[i - 1].position % 26 == 0 and self.rotors[i - 1].position != 0:
                self.rotors[i-1].position = 0
                self.rotors[i].rotate()
        newLine = 0
        for x in self.rotors:
            newLine=newLine+1
            print(x.position,end="\t")
            if(newLine%3==0): print()
            
            
enigma_machine = Enigma