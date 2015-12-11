package puzzle;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;
import java.util.TreeMap;

public class Tabuleiro {
    private int tabuleiro[][];
    private int linha_0;
    private int coluna_0;
    private String hash;
    private int g;
    private int h;
    private int[][] estadoFinal;
    
    public static final int TAM_TABULEIRO = 4;
    
    public Tabuleiro(int tabuleiro[][]) {
        this.tabuleiro = tabuleiro;
        this.gerarHash();
        this.hash = "-" + hash;
    }

    public Tabuleiro(int tabuleiro[][], int linha_0, int coluna_0, int estadoFinal[][]) {
        this(tabuleiro, linha_0, coluna_0, estadoFinal, 0);
    }
    
    public Tabuleiro(int tabuleiro[][], int linha_0, int coluna_0, int estadoFinal[][], int g) {
        this.tabuleiro = tabuleiro;
        this.linha_0 =  linha_0;
        this.coluna_0 =  coluna_0;
        this.g = g;
        this.estadoFinal = estadoFinal;
        this.gerarHash();
        this.h = this.heuristica3();
        appendHeuristicaHash();
    }
    
    private void gerarHash(){
        String hash = "";
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                hash += tabuleiro[i][j] + "-";
            }
        }
        this.hash = hash;
    }
    
    public String getHash() {
        return hash;
    }
    
    private void appendHeuristicaHash(){
        int f = this.h+this.g;
        hash = f + "-" + hash;
        if (f >= 10){
            hash = "a" + hash;
            if (f >= 100){
                hash = "a" + hash;
            }
        }
    }
    
    public boolean compara(Tabuleiro t){
        String s1 = t.getHash().substring(t.getHash().indexOf("-"));
        String s2 = this.getHash().substring(this.getHash().indexOf("-"));
        return s1.equals(s2);
    }
    
    private ArrayList<Tabuleiro> gerarSucessores(){
        ArrayList<Tabuleiro> sucessores = new ArrayList<>();
        if (coluna_0 > 0){
            sucessores.add(this.trocarPosicao0(linha_0, coluna_0 - 1));
        }
        if (coluna_0 < 3){
            sucessores.add(this.trocarPosicao0(linha_0, coluna_0 + 1));
        }
        if (linha_0 > 0){
            sucessores.add(this.trocarPosicao0(linha_0 - 1, coluna_0));
        }
        if (linha_0 < 3){
            sucessores.add(this.trocarPosicao0(linha_0 + 1, coluna_0));
        }
        return sucessores;
    }
    
    private Tabuleiro trocarPosicao0(int linha, int coluna){
        int tab[][] = this.copiaTabuleiro();
        tab[this.linha_0][this.coluna_0] = tab[linha][coluna];
        tab[linha][coluna] = 0;

        return new Tabuleiro(tab, linha, coluna, this.estadoFinal, this.g+1);
    }
    
    private int[][] copiaTabuleiro(){
        int novo[][] = new int[TAM_TABULEIRO][TAM_TABULEIRO];
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                novo[i][j] = this.tabuleiro[i][j];
            }
        }
        return novo;
    }
    
    //Heurística 1 - Número de peças fora do lugar
    private int heuristica1(){
        int cont = 0;
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                if (this.estadoFinal[i][j] != tabuleiro[i][j]){
                    cont++;
                }
            }
        }
        return cont;
    }
    
    //Heurística 3 - Distância retangular(Manhattan)
    private int heuristica3() {
        int posicao[][] = new int[TAM_TABULEIRO*TAM_TABULEIRO][2];
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                posicao[this.estadoFinal[i][j]][0] = i;
                posicao[this.estadoFinal[i][j]][1] = j;
            }
        }
        int heuristica = 0, posI, posJ;
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                posI = posicao[tabuleiro[i][j]][0];
                posJ = posicao[tabuleiro[i][j]][1];
                heuristica += Math.abs(i - posI) + Math.abs(j - posJ);
            }
        }
        
        return heuristica;
    }
    
    public void a_estrela(){
        Tabuleiro resposta = new Tabuleiro(this.estadoFinal);
        
        SortedMap<String, Tabuleiro> abertos =  new TreeMap<String, Tabuleiro>();
        Map<String, Tabuleiro> fechados =  new HashMap<String, Tabuleiro>();
        ArrayList<Tabuleiro> sucessores = null;
        
        boolean achou = false;
        abertos.put(this.hash, this);
        this.g = 0;
        
        Tabuleiro t;
        int movimentos = 0;
        int i = 0;
        while (!abertos.isEmpty() && !achou){
            System.out.println("I = "+i);
            i++;
            
            //Extrai o menor valor de f
            t = abertos.get(abertos.firstKey());

            abertos.remove(t.hash);
            
            if (t.compara(resposta)){
                movimentos = t.g;
                achou = true;
            }
            
            fechados.put(t.hash, t);

            sucessores = t.gerarSucessores();
            for (Tabuleiro sucessor : sucessores) {
                if (fechados.containsKey(sucessor.hash)){
                    continue;
                }
                
                int novo_g = t.g + 1;
                
                int antigo_g = abertos.containsKey(sucessor.hash) ? abertos.get(sucessor.hash).g : Integer.MAX_VALUE;
                if (novo_g >= antigo_g){
                    continue;
                }
                
                abertos.remove(sucessor.hash);

                sucessor.g = novo_g;
                abertos.put(sucessor.hash, sucessor);
            }
        }
        
        if (achou){
            System.out.println("ACHOU -> "+movimentos+"\nIteracoes -> "+i);
        } else {
            System.out.println("SEM SOLUÇÂO");
        }
    }
    
    public void imprime() {
        for (int i = 0; i < TAM_TABULEIRO; i++) {
            System.out.print("[");
            for (int j = 0; j < TAM_TABULEIRO; j++) {
                System.out.format("%3d ", tabuleiro[i][j]);
            }
            System.out.println("]");
        }
        System.out.println("HASH: " + hash);
        System.out.println("I = " + linha_0 + "\nJ = " + coluna_0);
        System.out.println();
    }
}
