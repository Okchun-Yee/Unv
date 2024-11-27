import heapq
import pickle
import os

# 허프만 트리 노드 클래스
class huffmanClass:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None
    
    def __lt__(self, other):
        return self.freq < other.freq

# 빈도수 계산
def mDictionary(text):
    charCnt = {}
    for char in text:
        if char in charCnt:
            charCnt[char] += 1
        else:
            charCnt[char] = 1
    return charCnt

# 허프만 트리 생성
def mhuffmanTree(charCnt):
    heap = [huffmanClass(char, freq) for char, freq in charCnt.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)

        merged = huffmanClass(None, left.freq + right.freq)
        merged.left = left
        merged.right = right

        heapq.heappush(heap, merged)

    return heap[0]

# 허프만 코드 생성
def mhuffmanCode(root, current_code="", codes={}):
    if root is None:
        return codes

    if root.char is not None:
        codes[root.char] = current_code

    mhuffmanCode(root.left, current_code + "0", codes)
    mhuffmanCode(root.right, current_code + "1", codes)

    return codes

# 압축된 텍스트 저장 (이진 데이터로)
def saveEncodedText(encoded_text, output_filename):
    byte_array = bytearray()
    for i in range(0, len(encoded_text), 8):
        byte_chunk = encoded_text[i:i+8]
        byte_value = int(byte_chunk, 2)
        byte_array.append(byte_value)
    with open(output_filename, 'wb') as fp:
        fp.write(byte_array)

# 허프만 트리 저장 (pickle 사용)
def saveHuffmanTree(root, tree_filename):
    with open(tree_filename, 'wb') as fp:
        pickle.dump(root, fp)

# 허프만 트리 로드 (pickle 사용)
def loadHuffmanTree(tree_filename):
    with open(tree_filename, 'rb') as fp:
        root = pickle.load(fp)
    return root

def decodeWithTree(encoded_text, root):
    decoded_text = []
    current_node = root
    for bit in encoded_text:
        if bit == '0':
            current_node = current_node.left
        else:
            current_node = current_node.right
        
        # 리프 노드에 도달하면 해당 문자를 결과에 추가
        if current_node.left is None and current_node.right is None:
            decoded_text.append(current_node.char)
            current_node = root  # 트리의 루트로 돌아가서 새로 시작

    return ''.join(decoded_text)