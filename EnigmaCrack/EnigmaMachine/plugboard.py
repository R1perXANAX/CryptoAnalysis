class Plugboard:
    
    def __init__(self, pairs):
        self.pairs = pairs

    def encode(self, char):
        for pair in self.pairs:
            if char == pair[0]:
                return pair[1]
            elif char == pair[1]:
                return pair[0]
        return char