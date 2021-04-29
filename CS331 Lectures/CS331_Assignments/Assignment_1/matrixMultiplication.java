
import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

class matrixMultiplicationThread implements Runnable
{
	public Thread thrd;

	private Integer threadID;
	public Integer threadCnt;
	
	private static final Object lock = new Object();
	private static Integer cnt = 0;

	public static Integer n = 1000;
	public static int[][] a = new int[n][n];
	public static int[][] b = new int[n][n];
	public static int[][] c = new int[n][n];

	public void init(Integer l, Integer r)
	{
		Random rand = new Random();
		for(int i = l; i < r; i++)
		{
			for(int j = 0; j < n; j++)
			{
				a[i][j] = rand.nextInt(11);
				b[i][j] = rand.nextInt(11);
				c[i][j] = 0;
			}
		}
	}

	public static void printMatrix(int[][] a)
	{
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < n; j++)
				System.out.print(a[i][j] + " ");
			System.out.println();
		}
		System.out.println();

	}

	public static void print()
	{
		printMatrix(a);
		printMatrix(b);
		printMatrix(c);
	}

	public matrixMultiplicationThread(Integer threadID, Integer threadCnt)
	{
		String name = "#Thread_" + Integer.toString(threadID);
		this.threadID = threadID;
		this.threadCnt = threadCnt;

		thrd = new Thread(this, name);
		System.out.println(name + " Starting.....");
		thrd.start();
	}

	public void multiply(int lb, int rb)
	{
		for(int i = lb; i < rb; i++)
			for(int j = 0; j < n; j++)
				for(int k = 0; k < n; k++)
					c[i][j] = c[i][j] + a[i][k] * b[k][j];
	}
	public void run()
	{
		int size = (n - 1 + threadCnt) / threadCnt;
		int l = size * threadID;
		int r = Math.min(n, l + size);
		init(l, r);
		synchronized(lock)
		{
			cnt = cnt + 1;
		}

		
		while(true)
		{
			synchronized(lock)
			{
				if(cnt >= threadCnt)
					break;
			}
		}

		multiply(l, r);

		System.out.println(this.thrd.getName() + " Finishing.....");
	}
}

class matrixMultiplication
{
	public static void main(String args[])
	{
		// Inconsistent number of arguments
		if(args.length != 1)
		{
			System.out.println("usage: java matrixMultiplication --threadCount[Integer[4-16]]");
			return;
		}

		try
		{
			Integer threadCnt = Integer.parseInt(args[0]);
			if(threadCnt <  4 || threadCnt > 16)
				throw new NumberFormatException();

			matrixMultiplicationThread[] threads = new matrixMultiplicationThread[threadCnt];
			for(int i = 0; i < threadCnt; i++)
				threads[i] = new matrixMultiplicationThread(i, threadCnt);

			try
			{
				for(int i = 0; i < threadCnt; i++)
					threads[i].thrd.join();

				matrixMultiplicationThread.print();
			}
			catch(InterruptedException exc)
			{
				System.out.println("Main thread interrupted.");
				return;
			}
			
		}
		catch(NumberFormatException numExc)
		{
			// Argument not a number or a number not in range
			System.out.println("usage: java matrixMultiplication --threadCount[Integer[4-16]]");
		}
		return;
	}
}
