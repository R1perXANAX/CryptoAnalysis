import networkx as nx

from  EnigmaMachine.enigma import enigma_machine

def create_crib_graph(crib, encrypted_text):
    G = nx.DiGraph()
    n = len(crib)
    encrypted_text = encrypted_text[:n]  # Considering only the first n characters
    for i, (p, c) in enumerate(zip(crib, encrypted_text)):
        if G.has_edge(p, c):
            G[p][c]['weight'].append(i)
        else:
            G.add_edge(p, c, weight=[i])
    return G, crib, encrypted_text

def crib_analysis(crib, encrypted_text):
    G, crib, encrypted_text = create_crib_graph(crib, encrypted_text)  # Unpack the tuple
    analysis_results = []
    n = len(crib)
    for i in range(n):
        plaintext_letter = crib[i]
        encrypted_letter = encrypted_text[i]
        if G.has_edge(plaintext_letter, encrypted_letter):
            analysis_results.append((plaintext_letter, encrypted_letter))
    return analysis_results


def create_gamma_k_graph(crib_analysis_results):
    G = nx.DiGraph()
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
    
    # Add edges for symmetry
    for L1 in alphabet:
        for L2 in alphabet:
            if L1 != L2:
                G.add_edge((L1, L2), (L2, L1))

    # Add edges based on crib analysis
    for L1, L2 in crib_analysis_results:
        print(L2)
        L3_transformed = enigma_machine.encode_letter(L2)
        G.add_edge((L1, L2), (L2, L3_transformed))

    return G