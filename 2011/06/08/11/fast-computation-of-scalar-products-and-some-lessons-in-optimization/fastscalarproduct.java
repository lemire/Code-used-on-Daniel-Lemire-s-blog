/**
* Fast computation of scalar products, and some lessons in optimization
* http://lemire.me/blog/archives/2011/08/11/fast-computation-of-scalar-products-and-some-lessons-in-optimization/
* 
*/

import java.util.*;
class fastscalarproduct {
	
	public final static float scalarProduct(float[]  v1, float[]  v2) {
		return v1[0] * v2[0]  + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
	}
	
	public final static float fastscalarProduct(float[]  v1, float b1, float[]  v2, float b2) {
		
		return ( v1[0] + v2[1] ) * ( v2[0]  + v1[1]) + ( v1[2] + v2[3] ) * ( v1[3] + v2[2]) -b1 - b2;
	}
	
	public static void main(String[]  args) {
		int N = 10000;
		float[][] vectors = new float[N][5];
		Random rand = new Random();
		float[] buffers = new float[N];
		for (int k = 0; k<N; ++k) {
			for(int l = 0; l<4; ++l) {
				vectors[k][l] = rand.nextFloat();
			}
			buffers[k] = vectors[k][0] * vectors[k][1] + vectors[k][2] * vectors[k][3];
		}
		float sumofproducts = 0;
		for(int k1 = 0; k1< N; ++k1) {
			for(int k2 = 0; k2< N; ++k2) {
				sumofproducts += scalarProduct(vectors[k1],vectors[k2]);
			}
		}	
		float sumofproducts2 = 0;
		for(int k1 = 0; k1< N; ++k1) {
			for(int k2 = 0; k2< N; ++k2) {
				sumofproducts2 += fastscalarProduct(vectors[k1],buffers[k1],vectors[k2],buffers[k2]);
			}
		}	
		long bef = System.currentTimeMillis();
		for(int k1 = 0; k1< N; ++k1) {
			for(int k2 = 0; k2< N; ++k2) {
				sumofproducts += scalarProduct(vectors[k1],vectors[k2]);
			}
		}	
		long aft1 = System.currentTimeMillis();		
		for(int k1 = 0; k1< N; ++k1) {
			for(int k2 = 0; k2< N; ++k2) {
				sumofproducts2 += fastscalarProduct(vectors[k1],buffers[k1],vectors[k2],buffers[k2]);
			}
		}	
		long aft = System.currentTimeMillis();
		System.out.println((aft1-bef)+" "+(aft-aft1));
		System.out.println(sumofproducts+" "+sumofproducts2);

	}
}