/*
 * Usage: 
 *   java -Xmx2G FastTableScanning
 *   
 *   This code was written by D. Lemire, see http://lemire.me/. 
 *   It was the basis of a blog post : "For your in-memory databases, do you really need an index?"
 *   http://lemire.me/blog/archives/2010/12/20/for-your-in-memory-databases-do-you-really-need-an-index/
 *
 *    It is in the public domain. (No copyright.)
 *   
 *   
 */

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Random;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicInteger;

public class FastTableScanning implements Iterable<int[]> {

	File backfile;
	public int c, N;
	public List<int[]> memoryversion;

	private FastTableScanning(int myN, int myc, int cardinality)
			throws IOException {
		backfile = File.createTempFile("PersistentNormalizedTable", "bin");
		backfile.deleteOnExit();// this defeats the purpose of "persistence" but
								// will do for our limited purposes
		N = myN;
		c = myc;
		Random rand = new Random();
		System.out.println("Creating on disk a table with " + c
				+ " columns and " + N + " rows... (this can take some time)");
		DataOutputStream dos = new DataOutputStream(new BufferedOutputStream(
				new FileOutputStream(backfile)));
		for (int row = 0; row < N; ++row) {
			for (int column = 0; column < c; ++column)
				dos.writeInt(rand.nextInt(cardinality));
		}
		dos.close();
		System.out
				.println("Done! Wrote " + N * c * 4 / (1024 * 1024.0) + " MB");
	}

	public static void main(String[] args) throws IOException,
			InterruptedException, BrokenBarrierException {
		FastTableScanning fs = new FastTableScanning(10000000, 10, 100);
		System.out
				.println("Now I'm going to repeatedly scan the data for slices using a memory-mapped file: ");
		for (int value = 0; value < 10; ++value)
			fs.computeCardinalityOfSlice(4, value);
		fs.loadInRam();
		System.out
				.println("Now I'm going to repeatedly scan the data for slices using an in-memory array: ");
		for (int value = 0; value < 10; ++value)
			fs.computeCardinalityOfSlice(4, value);

	}

	public void loadInRam() {
		System.out
				.println("Loading the entire table in RAM. You may need to adjust the flags on your JVM to make this work without OutOfMemoryError (hint: -Xmx2G may work).");
		memoryversion = new ArrayList<int[]>();
		for (int[] row : this) {
			memoryversion.add(Arrays.copyOf(row, row.length));
		}
		System.out.println("Done!");
	}

	public int computeCardinalityOfSlice(final int filtervalue,
			final int filtercolumn) throws InterruptedException,
			BrokenBarrierException {
		long startTime = System.currentTimeMillis();
		final int numberofthreads = Runtime.getRuntime().availableProcessors();
		final CyclicBarrier cb = new CyclicBarrier(numberofthreads + 1);
		final AtomicInteger counter = new AtomicInteger(0);
		for (int t = 0; t < numberofthreads; ++t) {
			final int ft = t;
			Thread r = new Thread() {
				@Override
				public void run() {
					try {
						if (memoryversion != null)
							processRAMPartition(counter, filtervalue,
									filtercolumn, ft, numberofthreads);
						else
							processDiskPartition(counter, filtervalue,
									filtercolumn, ft, numberofthreads);
					} catch (IOException e) {
						e.printStackTrace();
					}
					try {
						cb.await();
					} catch (InterruptedException e) {
						e.printStackTrace();
					} catch (BrokenBarrierException e) {
						e.printStackTrace();
					}
				}
			};
			r.start();
		}
		cb.await();
		long endTime = System.currentTimeMillis();
		System.out.println("Completed scan in  " + (endTime - startTime)
				/ 1000.0 + " s ");
		System.out.println("** The cardinality of the slice is " + counter
				+ " rows.");
		return counter.intValue();

	}

	// this is a silly example, modify to fit your needs
	// as it is, it computes the cardinality of a slice
	private void processDiskPartition(AtomicInteger counter, int filtervalue,
			int filtercolumn, final int whichthread, final int howmanythreads)
			throws IOException {
		final int startindex = startIndex(whichthread, howmanythreads);
		final int endindex = endIndex(whichthread, howmanythreads);
		Iterator<int[]> j = iteratorFromTo(startindex, endindex);
		while (j.hasNext()) {
			int[] row = j.next();
			if (row[filtercolumn] == filtervalue) {
				counter.incrementAndGet();
			}
		}

	}

	// this is a silly example, modify to fit your needs
	// as it is, it computes the cardinality of a slice
	private void processRAMPartition(AtomicInteger counter, int filtervalue,
			int filtercolumn, final int whichthread, final int howmanythreads)
			throws IOException {
		final int startindex = startIndex(whichthread, howmanythreads);
		final int endindex = endIndex(whichthread, howmanythreads);
		Iterator<int[]> j = memoryversion.subList(startindex, endindex)
				.iterator();
		while (j.hasNext()) {
			int[] row = j.next();
			if (row[filtercolumn] == filtervalue) {
				counter.incrementAndGet();
			}
		}

	}

	private int startIndex(final int whichthread, final int howmanythreads) {
		return N / howmanythreads * whichthread;
	}

	private int endIndex(final int whichthread, final int howmanythreads) {
		return whichthread + 1 == howmanythreads ? N : N / howmanythreads
				* (whichthread + 1);
	}

	public Iterator<int[]> iterator() {
		try {
			FileChannel roChannel = new RandomAccessFile(backfile, "r")
					.getChannel();

			ByteBuffer readonlybuffer = roChannel.map(
					FileChannel.MapMode.READ_ONLY, 0, c * N * 4);
			final IntBuffer ib = readonlybuffer.asIntBuffer();

			final int[] array = new int[c];
			final int endrowid = N;
			return new Iterator<int[]>() {
				int counter = 0;

				@Override
				public boolean hasNext() {
					if (counter++ < endrowid) {
						ib.get(array);
						return true;
					}
					return false;
				}

				@Override
				public int[] next() {
					return array;
				}

				@Override
				public void remove() {
					throw new RuntimeException("not implemented");
				}

			};
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		return null;
	}

	public Iterator<int[]> iteratorFromTo(final int beginrowid,
			final int endrowid) throws IOException {
		FileChannel roChannel = new RandomAccessFile(backfile, "r")
				.getChannel();

		ByteBuffer readonlybuffer = roChannel.map(
				FileChannel.MapMode.READ_ONLY, c * beginrowid * 4, c
						* (endrowid - beginrowid) * 4);
		final IntBuffer ib = readonlybuffer.asIntBuffer();
		final int[] array = new int[c];
		return new Iterator<int[]>() {
			int counter = beginrowid;

			@Override
			public boolean hasNext() {
				if (counter++ < endrowid) {
					ib.get(array);
					return true;
				}
				return false;
			}

			@Override
			public int[] next() {
				return array;
			}

			@Override
			public void remove() {
				throw new RuntimeException("not implemented");
			}

		};
	}

}