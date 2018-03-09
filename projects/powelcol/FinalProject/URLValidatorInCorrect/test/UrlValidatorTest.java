/*
 * CS362
 * Final Project: Part B
 * Sarah Maas (maass), Colin Powell (powelcol), Josh Williams (willijo4)
 */

import org.junit.Rule;
import org.junit.Test;
//import junit.framework.TestCase;
import org.junit.rules.ErrorCollector;
import org.hamcrest.CoreMatchers;

public class UrlValidatorTest {
	
	@Rule
	public ErrorCollector collector = new ErrorCollector();

	@Test
	public void testResultPairs() {
		// ResultPair is broken...see ResultPair.java in src dir.
		ResultPair rp_true = new ResultPair("http://www.google.com", true);
		ResultPair rp_false = new ResultPair("foo", false);
//		boolean pass = true;   
				
//		System.out.println("Testing ResultPair objects...");
		collector.checkThat("ResultPair reporting incorrect valid property", rp_true.valid, CoreMatchers.equalTo(true));
		collector.checkThat("ResultPair reporting incorrect valid property", rp_false.valid, CoreMatchers.equalTo(false));
/*				
		if(rp_true.valid != true) {
			System.out.println("mismatch in ResultPair --> expected: " + true + " got: " + rp_true.valid);
			pass = false;
		}
		if(rp_false.valid != false) {
			System.out.println("mismatch in ResultPair --> expected: " + false + " got: " + rp_false.valid);		   
			pass = false;
		}		
		if(pass) {
			System.out.println("--ResultPair tests OK");
		}
*/
	}
	
	@Test
   	public void testManualTest() {

   		System.out.println("********Manual Tests********");
		
		System.out.println("Testing URLs...");
		String[] schemes = { "http", "https", "ftp" };
   		UrlValidator urlVal = new UrlValidator(schemes);
   	
   		// basic manual tests: 
   		// DEBUGGING NOTE: check UrlValidator line 282; schemes are converted to upper case 
   		//				   instead of lower case, as required by isValid...
   		//				   if ALLOW_ALL_SCHEMES flag is not set, this will cause all tests below to fail.
   		
   		collector.checkThat("expect basic http-based URL to be valid", 
   				  	urlVal.isValid("http://www.google.com"), CoreMatchers.equalTo(true));
   		
   		collector.checkThat("expect https to be recognized", 
   					urlVal.isValid("https://www.google.com"), CoreMatchers.equalTo(true));
   		
   		collector.checkThat("expect port numbers to be valid", 
   				  	urlVal.isValid("http://www.google.com:65530"), CoreMatchers.equalTo(true));
   		
   		collector.checkThat("expect query string to register as valid", 
   				  	urlVal.isValid("http://www.google.com?foo=bar&baz=bam"), CoreMatchers.equalTo(true));
   		
   		//RFC 1123: hostnames must be alphanumeric:
   		
   		collector.checkThat("expect numeric chars in hostname to register as valid", 
   				  	urlVal.isValid("http://www.g00gle.com"), CoreMatchers.equalTo(true));
   		
   		collector.checkThat("expect invalid TLD chars to register as invalid", 
   				  	urlVal.isValid("http://www.google.c!m"), CoreMatchers.equalTo(false));
   		
   		collector.checkThat("expect invalid hostname chars to register as invalid", 
   					urlVal.isValid("http://www.g!.com"), CoreMatchers.equalTo(false));
   		 		
   		// non-http protocols seem to be an issue, esp. with ALLOW_ALL_SCHEMES flag set:
   		// wrap in try/catch blocks to catch errors without terminating program
   		try {
   			collector.checkThat("expect ftp scheme to register as valid", 
   					urlVal.isValid("ftp://ftp.filestorage.com:5440"), CoreMatchers.equalTo(true));
   		} 
   		catch(Throwable err) {
   			collector.addError(err);
   		}
  
 	 	try {
 	 		collector.checkThat("expect ftp scheme to register as valid", 
 	 				urlVal.isValid("ftp://foo.bar.com/"), CoreMatchers.equalTo(true));
 	 	}
 	 	catch(Throwable err) {
 	 		collector.addError(err);
 	 	}		
	   
   		System.out.println("Manual Tests Complete");
   }
   
   @Test
   public void testYourFirstPartition() {
	 //You can use this function to implement your First Partition testing	   

   }
   
   @Test
   public void testYourSecondPartition() {
		 //You can use this function to implement your Second Partition testing	   

   }
   //You may need to create more test cases for your Partitions
   
   @Test
   public void testIsValid() {
	   //You can use this function for programming based testing

   }

}
