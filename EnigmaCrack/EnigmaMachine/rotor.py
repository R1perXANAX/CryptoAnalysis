class Rotor:
    
    def __init__(self, perm, position,name):
        self.perm = perm
        self.position = position
        self.name=name
        
    def rotate(self):
        self.position = (self.position + 1)
        
    def encodeRotor(self, letter):
        newLetter=chr(ord(letter)+self.position)
        
        if newLetter > 'Z':
            newLetter = chr(ord(newLetter) - 26)
            
        newLetter=self.perm[ord(newLetter)-ord('A')]
        newLetter=chr(ord(newLetter)-self.position)
        
        if newLetter < 'A':
            newLetter = chr(ord(newLetter)+26)
            
        return newLetter
    
    def reverseEncode(self, letter):
        newLetter=chr(ord(letter)+self.position)
        
        if newLetter > 'Z':
            newLetter = chr(ord(newLetter) - 26)
            
        newLetter=chr(ord('A')+self.perm.index(newLetter))
        newLetter = chr(ord(newLetter) - self.position)
        
        if newLetter < 'A':
            newLetter = chr(ord(newLetter) + 26)
            
        return newLetter