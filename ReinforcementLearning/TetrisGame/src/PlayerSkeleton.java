import java.util.*;
import java.io.*;



public class PlayerSkeleton {
	//implement this function to have a working system
		public int pickMove(State s, int[][] legalMoves) {
		Random generator = new Random();
		int randomIndex = generator.nextInt(legalMoves.length);
		return randomIndex;
	}
	
	public static void main(String[] args) {
		PlayerSkeleton p = new PlayerSkeleton();
		State s = new State();
		TFrame t = new TFrame(s);

		while(!s.hasLost()) {
			s.makeMove(p.pickMove(s,s.legalMoves()));
			s.draw();
			s.drawNext(0,0);
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		System.out.println(s.getRowsCleared());
	}
}