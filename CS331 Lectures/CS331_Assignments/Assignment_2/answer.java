import java.util.concurrent.locks.ReentrantLock;
import java.util.*;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.*;

class Account
{
	long aNum;
	long balance;
	boolean marked;
	ReentrantLock lock = new ReentrantLock();
	Account next = null;
	Account(long balance, long aNum)
	{
		this.aNum = aNum;
		this.balance = balance;
		this.marked = false;
	}
}

class list
{
	Account head;
	Account tail;

	list()
	{
		head = new Account(0, Long.MIN_VALUE);
		head.next = new Account(0, Long.MAX_VALUE);
		tail = head.next;
	}
	
	synchronized public void print()
	{
		Account temp = head.next;
		while(temp.aNum != Long.MAX_VALUE)
		{
			System.out.println(temp.balance + " " + temp.aNum);
			temp = temp.next;
		}
	}

	public boolean validate(Account prev, Account curr)
	{
		return !prev.marked && !curr.marked && prev.next == curr;
	}

	public boolean add(long balance, long aNum)
	{
		while(true)
		{
			Account prev = head;
			Account curr = head.next;
			
			prev.lock.lock();
			try
			{
				curr.lock.lock();
				try
				{
					if(validate(prev, curr))
					{
						Account account = new Account(balance, aNum);
						account.next = curr;
						prev.next = account;
						return true;
					}
				}
				finally
				{
					curr.lock.unlock();
				}
			}
			finally
			{
				prev.lock.unlock();
			}
		}
	}

	public Account find(long aNum)
	{
		Account curr = head;
		while(curr.aNum != Long.MAX_VALUE && curr.aNum != aNum)
		{
			curr = curr.next;
		}
		if(curr.aNum == aNum && !curr.marked)
		{
			return curr;
		}
		return null;
	}

	public boolean remove(long aNum)
	{
		while(true)
		{
			Account prev = head;
			Account curr = head.next;
			while(curr.aNum != Long.MAX_VALUE && curr.aNum != aNum)
			{
				prev = curr;
				curr = curr.next;
			}
			prev.lock.lock();
			try
			{
				curr.lock.lock();
				try
				{
					if(validate(prev, curr))
					{
						if(curr.aNum != aNum)
							return false;
						else
						{
							curr.marked = true;
							prev.next = curr.next;
							return true;
						}
					}
				}
				finally
				{
					curr.lock.unlock();
				}
			}
			finally
			{
				prev.lock.unlock();
			}
		}
	}
	

}

class Bank
{
	public static HashMap<Integer, list> branches = new HashMap<>();
	public static ArrayList<ExecutorService> updaters = new ArrayList<>();
	public static boolean verbose = false;

	public static int bID(long number)
	{
		return (int)(number / (1000000000L));
	}

	public static boolean deposit(long amount, long aNum)
	{
		list branch = branches.get(bID(aNum));
		Account account = branch.find(aNum);
		if(account != null)
		{
			account.lock.lock();
			try
			{
				account.balance += amount;
				return true;
			}
			finally
			{
				account.lock.unlock();
			}
		}
		return false;
	}

	public static boolean withdraw(long amount, long aNum)
	{
		list branch = branches.get(bID(aNum));
		Account account = branch.find(aNum);
		if(account != null)
		{
			account.lock.lock();
			try
			{
				if(account.balance >= amount)
				{
					account.balance -= amount;
					return true;
				}
				return false;
			}
			finally
			{
				account.lock.unlock();
			}
		}
		return false;
	}

	public static boolean transfer(long amount, long aNum1, long aNum2)
	{
		if(withdraw(amount, aNum1))
		{
			return deposit(amount, aNum2);
		}
		return false;
	}

	public static boolean addAccount(long amount, long aNum)
	{
		list branch = branches.get(bID(aNum));
		return branch.add(amount, aNum);
	}

	public static boolean deleteAccount(long aNum)
	{
		list branch = branches.get(bID(aNum));
		return branch.remove(aNum);
	}

	public static boolean transferAccount(long aNum1, long aNum2)
	{
		list branch1 = branches.get(bID(aNum1));
		Account account = branch1.find(aNum1);
		if(account != null)
		{
			long amount;
			account.lock.lock();
			try
			{
				amount = account.balance;
			}
			finally
			{
				account.lock.unlock();
			}
			if(deleteAccount(aNum1))
			{
				return addAccount(amount, aNum2);
			}
		}
		return false;
	}

}

class Deposit implements Runnable
{
	long amount;
	long aNum;
	Deposit(long amount, long aNum)
	{
		this.amount = amount;
		this.aNum = aNum;
	}
	public void run()
	{
		Bank.deposit(amount, aNum);
		if(Bank.verbose)
			System.out.println("Deposit " + amount + " " + aNum);
	}
}

class Withdraw implements Runnable
{
	long amount;
	long aNum;
	Withdraw(long amount, long aNum)
	{
		this.amount = amount;
		this.aNum = aNum;
	}
	public void run()
	{
		Bank.withdraw(amount, aNum);
		if(Bank.verbose)
			System.out.println("Withdraw " + amount + " " + aNum);
	}
}

class Transfer implements Runnable
{
	long amount;
	long aNum1;
	long aNum2;
	Transfer(long amount, long aNum1, long aNum2)
	{
		this.amount = amount;
		this.aNum1 = aNum1;
		this.aNum2 = aNum2;
	}
	public void run()
	{
		Bank.transfer(amount, aNum1, aNum2);
		if(Bank.verbose)
			System.out.println("Transfer " + amount + " " + aNum1 + " " + aNum2);
	}
}

class AddCustomer implements Runnable
{
	long amount;
	long aNum;
	AddCustomer(long amount, long aNum)
	{
		this.amount = amount;
		this.aNum = aNum;
	}
	public void run()
	{
		Bank.addAccount(amount, aNum);
		if(Bank.verbose)
			System.out.println("AddCustomer " + amount + " " + aNum);
	}
}

class DeleteCustomer implements Runnable
{
	long aNum;
	DeleteCustomer(long aNum)
	{
		this.aNum = aNum;
	}
	public void run()
	{
		Bank.deleteAccount(aNum);
		if(Bank.verbose)
			System.out.println("DeleteCustomer " + aNum);
	}
}

class TransferCustomer implements Runnable
{
	long aNum1;
	long aNum2;
	TransferCustomer(long aNum1, long aNum2)
	{
		this.aNum1 = aNum1;
		this.aNum2 = aNum2;
	}

	public void run()
	{
		Bank.transferAccount(aNum1, aNum2);
		if(Bank.verbose)
			System.out.println("TransferCustomer " + aNum1 + " " + aNum2);
		
	}
}

class Main
{
	public static void main(String args[])
	{
		// Vebose mode for printing details
		if(args.length == 1)
		{
			System.out.println(args[0]);
			if(args[0].equals("verbose"))
				Bank.verbose = true;
		}
		// Create branches
		for(int i = 0; i < 10; i++)
		{
			list branch = new list();
			Bank.branches.put(i, branch);
		}

		// Create users
		ArrayList<ArrayList<Long>> users = new ArrayList<>();
		for(int i = 0; i < 10; i++)
		{
			ArrayList<Long> branch = new ArrayList<>();
			for(int j = 0; j < 10000; j++)
			{
				long account = i * 1000000000L + j;
				branch.add(account);
			}
			Collections.shuffle(branch);
			users.add(branch);
		}

		// Add initial users to branch
		Random rand = new Random();
		for(ArrayList<Long> branch: users)
		{
			for(long account: branch)
			{
				long balance = rand.nextInt(2000);
				Bank.addAccount(balance, account);
			}
		}


		if(Bank.verbose)
		{
			// Printing initial state
			for(int i = 0; i < 10; i++)
			{
				list branch = Bank.branches.get(i);
				branch.print();
			}
		}


		// Creating Exection Pools
		for(int i = 0; i < 10; i++)
		{
			ExecutorService bUpdaters = Executors.newFixedThreadPool(10);
			Bank.updaters.add(bUpdaters);
		}


		// A counter
		long counter = 10000;

		// Timer started
		long startTime = System.nanoTime();

		// Transactions
		for(int i = 0; i < 10000000; i++)
		{

			double sample = rand.nextDouble();

			if(sample <= 0.33)
			{
				int bID = rand.nextInt(10);
				int amount = rand.nextInt(100);
				ArrayList<Long> branch = users.get(bID); 
				long aNum = branch.get(rand.nextInt(branch.size()));

				Deposit request = new Deposit(amount, aNum);
				Bank.updaters.get(bID).execute(request);
			}
			else if(sample <= 0.66)
			{
				int bID = rand.nextInt(10);
				int amount = rand.nextInt(100);
				ArrayList<Long> branch = users.get(bID); 
				long aNum = branch.get(rand.nextInt(branch.size()));

				Withdraw request = new Withdraw(amount, aNum);
				Bank.updaters.get(bID).execute(request);
			}
			else if(sample <= 0.99)
			{
				int bID1 = rand.nextInt(10);
				int bID2 = rand.nextInt(10);
				int amount = rand.nextInt(100);

				ArrayList<Long> branch1 = users.get(bID1); 
				ArrayList<Long> branch2 = users.get(bID2); 
				
				long aNum1 = branch1.get(rand.nextInt(branch1.size()));
				long aNum2 = branch2.get(rand.nextInt(branch2.size()));


				Transfer request = new Transfer(amount, aNum1, aNum2);
				Bank.updaters.get(bID1).execute(request);
			}
			else if(sample <= 0.993)
			{
				int bID = rand.nextInt(10);
				int amount = rand.nextInt(100);

				long aNum = bID * 1000000000L + counter;
				counter++;

				users.get(bID).add(aNum);


				AddCustomer request = new AddCustomer(amount, aNum);
				Bank.updaters.get(bID).execute(request);
			}
			else if(sample <= 0.996)
			{
				int bID = rand.nextInt(10);
				ArrayList<Long> branch = users.get(bID); 
				long aNum = branch.get(rand.nextInt(branch.size()));

				users.get(bID).remove(aNum);

				DeleteCustomer request = new DeleteCustomer(aNum);
				Bank.updaters.get(bID).execute(request);
			}
			else
			{
				int bID1 = rand.nextInt(10);
				int bID2 = rand.nextInt(10);

				ArrayList<Long> branch1 = users.get(bID1);
				long aNum1 = branch1.get(rand.nextInt(branch1.size()));

				users.get(bID1).remove(aNum1);

				long aNum2 = bID2 * 1000000000L + counter;
				counter++;
				
				users.get(bID2).add(aNum2);

				TransferCustomer request = new TransferCustomer(aNum1, aNum2);
				Bank.updaters.get(bID1).execute(request);


			}
		}
		

		// Closing Execution Pools
		for(int i = 0; i < 10; i++)
		{
			Bank.updaters.get(i).shutdown();
			try
			{
				if (!Bank.updaters.get(i).awaitTermination(5000, TimeUnit.SECONDS)) 
				{
					System.out.println("Branch[" + i + "] Closing Abrubtly...");
					Bank.updaters.get(i).shutdownNow();
				}
			}
			catch (InterruptedException ex) 
			{
				System.out.println("Error");
				Bank.updaters.get(i).shutdownNow();
				Thread.currentThread().interrupt();
			}
			
			
		}

		if(Bank.verbose)
		{
			// Printing final state
			for(int i = 0; i < 10; i++)
			{
				list branch = Bank.branches.get(i);
				branch.print();
			}
		}
		
		// Timing the Execution
		long endTime = System.nanoTime();
		long duration = endTime - startTime;
		duration /= 1000000000L;
		System.out.println("Duration: " + duration);
		return;
	}
}
