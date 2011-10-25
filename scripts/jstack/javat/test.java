

public class test
{
  public static int res;

  public static void main(String [] args)
  {
    for(int i = 0; i < 3; i++) {
      Thread t = new Thread() {
	  public void run() {
	     int n = 0;
	     while(true) {
		 for(int i=0;i<100000;i++) {
		    n += i * 1;
		 }
		 res = n;
		 try {
  		   Thread.sleep( 10 );
		 } catch(Exception ex) {
		 }
	     }
	  }
	};
	t . start();
     }
  }
}
