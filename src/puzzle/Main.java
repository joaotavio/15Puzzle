package puzzle;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;


public class Main {

    public static void main(String[] args) throws IOException {
        int resp[][] = {{1,2,3,4},{12,13,14,5},{11,0,15,6},{10,9,8,7}};
        //int resp[][] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,0}};
        
        Tabuleiro t2 = lerArquivo("Caso2.txt", resp);
        t2.imprime();
        
        t2.a_estrela();
    }
    
    public static Tabuleiro lerArquivo(String nomeArquivo, int resp[][]) throws FileNotFoundException, IOException{        
        int tab[][] = new int[Tabuleiro.TAM_TABULEIRO][Tabuleiro.TAM_TABULEIRO];
        int linha0 = 0, coluna0 = 0;
        
        FileInputStream stream = new FileInputStream(nomeArquivo);
        InputStreamReader reader = new InputStreamReader(stream);
        //InputStreamReader reader = new InputStreamReader(System.in);
        BufferedReader br = new BufferedReader(reader);
       
        String linha = br.readLine();
        String strNums[] = linha.split(" ");
        int k = 0;
        for (int i = 0; i < Tabuleiro.TAM_TABULEIRO; i++) {
            for (int j = 0; j < Tabuleiro.TAM_TABULEIRO; j++) {
                tab[i][j] = Short.parseShort(strNums[k]);
                if (tab[i][j] == 0){
                    linha0 = i;
                    coluna0 = j;
                }
                k++;
            }        
        }       

        Tabuleiro tabuleiro = new Tabuleiro(tab, linha0, coluna0, resp);
        return tabuleiro;
    }
    
}
