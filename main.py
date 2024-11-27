import huffman
import os
from tkinter import *
from tkinter.filedialog import askopenfilename

def open_file():
    filename = askopenfilename(title="파일 열기", filetypes=[("텍스트 파일", "*.txt"), ("모든 파일", "*.*")])
    print("열린 파일:", filename)
    return filename

def fileopenCheck(filename):
    print("현재 디렉토리:", os.getcwd())
    try:
        with open(filename, 'r', encoding='utf-8') as fp:
            print("file open!\n")
            text = fp.read()
    except FileNotFoundError:
        print(f"파일 '{filename}'을(를) 찾을 수 없습니다.")
    except UnicodeDecodeError:
        print(f"파일 '{filename}'을(를) 읽을 수 없습니다. 인코딩 문제일 수 있습니다.")
    
    return text

def encoded(text):
    # 수정된 텍스트 (특수문자 따로 처리하지 않음)
    modifiedText = text  # 특수 문자 그대로 유지
    # 빈도수 계산
    frequency = huffman.mDictionary(modifiedText)
    # 허프만 트리 생성
    huffman_tree = huffman.mhuffmanTree(frequency)
    # 허프만 코드 생성
    huffman_code = huffman.mhuffmanCode(huffman_tree)
    
   # 압축된 텍스트 이진수로 변환 (허프만 코드 직접 사용)
    encodedText = ''.join([huffman_code[char] for char in modifiedText])
    # 압축된 텍스트 저장
    huffman.saveEncodedText(encodedText, "outputfile.bin")
    huffman.saveHuffmanTree(huffman_tree, "huffman_tree.pkl")

    return huffman_code


def main():
    filename = open_file()
    if(filename!=None):
        fileText = fileopenCheck(filename)
        huffman_code = encoded(fileText)
        
        print("압축된 텍스트를 : 'outputfile.bin'에 저장하였습니다.")
        print("허프만 트리를   : 'huffman_tree.pkl'에 저장하였습니다.")

main()
