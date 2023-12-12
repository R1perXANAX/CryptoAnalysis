class Reflector:
    
    def __init__(self, permutations):
        self.permutations = permutations

    def reflect(self, letter):
        letter=self.permutations[ord(letter)-ord('A')]
        return letter