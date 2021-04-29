import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

class simpsonRuleThread implements Runnable
{
	public Thread thrd;

	private Integer threadID;
	public Integer threadCnt;
	public Double area;
	private static Integer N = 2000000;

	private static final Object lock = new Object();

	public simpsonRuleThread(Integer threadID, Integer threadCnt)
	{
		String name = "#Thread_" + Integer.toString(threadID);
		this.threadID = threadID;
		this.threadCnt = threadCnt;
		this.area = 0.0;

		thrd = new Thread(this, name);
		System.out.println(name + " Starting.....");
		thrd.start();
	}

	Double f(Double x)
	{
		return Math.exp(-1.00 * (x * x / 2.00)) / Math.sqrt(2 * Math.PI);
	}

	void simpson(Integer l, Integer r)
	{
		Double h = 2.00 / N;
		for(int i = l; i < r; i++)
		{
			if(i == 0)
				area = area + f(-1.00);
			else if(i == N)
				area = area + f(1.00);
			else
			{
				Double x = -1.00 + i * h;
				if(i % 2 == 0)
					area = area + 2 * f(x);
				else
					area = area + 4 * f(x);
			}
		}
		area = area * h;
		area = area / 3;
	}


	public void run()
	{
		Integer size = (N + threadCnt) / threadCnt;
		Integer l = size * threadID;
		Integer r = Math.min(N + 1, l + size);
		simpson(l, r);
		System.out.println(this.thrd.getName() + " Finishing.....");
	}
}

class simpsonRule
{
	public static void main(String args[])
	{
		// Inconsistent number of arguments
		if(args.length != 1)
		{
			System.out.println("usage: java simpsonRule --threadCount[Integer[4-16]]");
			return;
		}

		try
		{
			Integer threadCnt = Integer.parseInt(args[0]);
			if(threadCnt <  4 || threadCnt > 16)
				throw new NumberFormatException();

			simpsonRuleThread[] threads = new simpsonRuleThread[threadCnt];
			for(int i = 0; i < threadCnt; i++)
				threads[i] = new simpsonRuleThread(i, threadCnt);

			try
			{
				for(int i = 0; i < threadCnt; i++)
					threads[i].thrd.join();
				Double area = 0.0;
				for(int i = 0; i < threadCnt; i++)
					area = area + threads[i].area;

				System.out.println("Area = " +  area);
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
			System.out.println("usage: java simpsonRule --threadCount[Integer[4-16]]");
		}
		return;
	}
}
