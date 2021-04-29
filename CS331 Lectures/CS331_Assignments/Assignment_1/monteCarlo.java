import java.util.Random;
import java.util.concurrent.atomic.AtomicInteger;

class monteCarloThread implements Runnable
{
	public Thread thrd;

	private Integer threadID;
	private static AtomicInteger circlePoints = new AtomicInteger(0);
	private static Integer points = 0;
	private static final Integer maxPoints = 1000000;

	private static final Object lock = new Object();

	public monteCarloThread(Integer threadID)
	{
		String name = "#Thread_" + Integer.toString(threadID);
		this.threadID = threadID;

		thrd = new Thread(this, name);
		System.out.println(name + " Starting.....");
		thrd.start();
	}

	private double dist(double x, double y)
	{
		return x * x + y * y;
	}

	public static void printResult()
	{
		System.out.println("Total Points = " + Integer.toString(points));
		System.out.println("Points inside circle = " + Integer.toString(circlePoints.get()));
		System.out.println("The value of pi = " + Double.toString(4.00 * circlePoints.get() / maxPoints));
	}

	public void run()
	{
		Random rand = new Random();
		Boolean flag = true;
		while(flag)
		{
			synchronized(lock)
			{
				if(points >=  maxPoints)
					break;
			}

			
			double x = -1.00 + 2.00 * Math.random();
			double y = -1.00 + 2.00 * Math.random();
			if(dist(x, y) <= 1)
			{
				synchronized(lock)
				{
					if(points < maxPoints)
					{
						points++;
						circlePoints.getAndIncrement();
					}	
				}
			}
			else
			{
				synchronized(lock)
				{
					if(points < maxPoints)
					{
						points++;
					}	
				}
			}
			
		}	
		System.out.println(this.thrd.getName() + " finishing.....");
	}
}

class monteCarlo
{
	public static void main(String args[])
	{
		// Inconsistent number of arguments
		if(args.length != 1)
		{
			System.out.println("usage: java monteCarlo --threadCount[Integer[4-16]]");
			return;
		}

		try
		{
			Integer threadCnt = Integer.parseInt(args[0]);
			if(threadCnt <  4 || threadCnt > 16)
				throw new NumberFormatException();

			monteCarloThread[] threads = new monteCarloThread[threadCnt];
			for(int i = 0; i < threadCnt; i++)
				threads[i] = new monteCarloThread(i);

			try
			{
				for(int i = 0; i < threadCnt; i++)
					threads[i].thrd.join();
			}
			catch(InterruptedException exc)
			{
				System.out.println("Main thread interrupted.");
			}


			monteCarloThread.printResult();
			
		}
		catch(NumberFormatException numExc)
		{
			// Argument not a number or a number not in range
			System.out.println("usage: java monteCarlo --threadCount[Integer[4-16]]");
		}
		return;
	}
}
