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
	
	boolean PRINT_FAILURES = true;
	
	@Rule
	public ErrorCollector collector = new ErrorCollector();

	@Test
	public void testResultPairs() {
		
   		System.out.println("********Manual ResultPair Tests********");

		// ResultPair is broken...see ResultPair.java in src dir.
		ResultPair rp_true = new ResultPair("http://www.google.com", true);
		ResultPair rp_false = new ResultPair("foo", false);
				
		System.out.println("Testing ResultPair objects...");
		collector.checkThat("ResultPair reporting incorrect valid property", rp_true.valid, CoreMatchers.equalTo(true));
		collector.checkThat("ResultPair reporting incorrect valid property", rp_false.valid, CoreMatchers.equalTo(false));
		System.out.println("ResultPair testing complete.\n");
	}
	
	@Test
   	public void testManualTest() {

   		System.out.println("********Manual URL Tests********");
		
   		boolean validTest = false;
   		
		System.out.println("Testing URLs...");
		String[] schemes = { "http", "https", "ftp" };
   		
		UrlValidator validator = new UrlValidator(schemes);
   		UrlValidator validatorAllSchemes = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);
   
   		// basic manual tests: 
   		// DEBUGGING NOTE: check UrlValidator line 282; schemes are converted to upper case 
   		// 		instead of lower case, as required by isValid...
   		// 		if ALLOW_ALL_SCHEMES flag is not set, this will cause all tests below to return invalid.
   		
	 	// DEBUGGING NOTE: line 318 in UrlValidator.java
	 	// 		UrlValidator.java written such that "http" cannot allow trailing ':' in authority.
	 	//		this should only be true for "file" schemes.
   		
   		// non-http protocols seem to be an issue, esp. with ALLOW_ALL_SCHEMES flag set:
   		// wrap in try/catch blocks to catch errors without terminating program
   		
   		System.out.println("Testing with specified http, https, ftp schemes: ");
   		
   		TestTuple [] urls = { 	
   								new TestTuple("basic http URL test", "http://www.google.com", true),
   								new TestTuple("basic https URL test", "https://www.google.com", true),
   								new TestTuple("basic ftp URL test", "ftp://foo.bar.com", true),
   								
   								new TestTuple("ports should be recognized", "http://www.google.com:65545", true),
   								new TestTuple("invalid port cannot be in URL", "http://www.google.com:-1", false),

   								// RFC 1123: hostnames must be alphanumeric
   								new TestTuple("special chars cannot be in domain", "http://www.g!.com", false),
   								new TestTuple("special chars cannot be in TLD", "http://www.google.c!m", false),
   								new TestTuple("numeric chars allowed in hostname", "http://www.g00gle.com", true),
   								
   								new TestTuple("query string should be recognized", "http://www.google.com?foo=bar&baz=bam", true)
   							};

   		/*
   		* loop through manual tests 
   		* don't decompose into a single loop to give more detailed / informative error handling
   		*/
   	
   		for( int i = 0; i < urls.length; i++ ) {		   			
   			// wrap in try/catch to handle errors (instead of test failures)		
   			try {			
   	   			validTest = validator.isValid(urls[i].getItem());
	   			// collect test data for each URL under test
	   			collector.checkThat( urls[i].getPurpose(), validTest, CoreMatchers.equalTo(urls[i].getValid()) );
	 
	   			if(PRINT_FAILURES && (validTest != urls[i].getValid())) {	   				
	   				System.out.println("--FAILURE: " + urls[i].getPurpose() + 
	   								   " --> URL: " + urls[i].getItem() + 
	   								   ", Expected: " + urls[i].getValid() + 
	   								   ", Observed: " + validTest);
	   			}
   			} 
   			catch (Throwable err) {  		
   				collector.addError(err);		
   			}  			
   		}
   		
   		System.out.println("Testing with ALLOW_ALL_SCHEMES set:");
   		
   		for( int i = 0; i < urls.length; i++ ) {		   			
   			try {			
   	   			validTest = validatorAllSchemes.isValid(urls[i].getItem());
	   			collector.checkThat( urls[i].getPurpose(), validTest, CoreMatchers.equalTo(urls[i].getValid()) );
	 
	   			if(PRINT_FAILURES && (validTest != urls[i].getValid())) {   				
	   				System.out.println("--FAILURE: " + urls[i].getPurpose() + 
	   								   " --> URL: " + urls[i].getItem() + 
	   								   ", Expected: " + urls[i].getValid() + 
	   								   ", Observed: " + validTest);
	   			}
   			} 
   			catch (Throwable err) {  		
   				collector.addError(err);		
   			}  			
   		}	
   		
   		
   		System.out.println("Manual testing complete\n");
 
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
