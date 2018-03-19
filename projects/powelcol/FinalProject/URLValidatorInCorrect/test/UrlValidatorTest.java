/*
 * CS362
 * Final Project: Part B
 * Team: Sarah Maas (maass), Colin Powell (powelcol), Josh Williams (willijo4)
 * Description: Testing of the Apache Commons URL Validator
 */

import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.hamcrest.CoreMatchers;

public class UrlValidatorTest {
	
	boolean PRINT_FAILURES = true;
	
	@Rule
	public ErrorCollector collector = new ErrorCollector();

	@Test
	public void testResultPairs() {
		
   		System.out.println("MANUAL TESTING: ResultPair Tests");

		// ResultPair is broken...see ResultPair.java in src dir.
		ResultPair rp_true = new ResultPair("http://www.google.com", true);
		ResultPair rp_false = new ResultPair("foo", false);
				
		System.out.println("Testing ResultPair objects...");
		collector.checkThat("ResultPair reporting incorrect valid property", rp_true.valid, CoreMatchers.equalTo(true));
		collector.checkThat("ResultPair reporting incorrect valid property", rp_false.valid, CoreMatchers.equalTo(false));
		System.out.println("ResultPair testing complete. See JUnit output for details.\n");
	}
	
	@Test
	public void manualTestDefaultSchemes() {
		
   		/* 
   		 * basic manual tests, http/https/ftp allowed ONLY
   		 * CGP DEBUGGING NOTE: check UrlValidator line 282; schemes are converted to upper case 
   		 *		instead of lower case, as required by isValid...
   		 *		if ALLOW_ALL_SCHEMES flag is not set, this will cause all tests below to return invalid.
		 */
		
		System.out.println("MANUAL TESTING: Specifying list of default schemes (http, https, ftp schemes)");
   	  
   		boolean validTest = false;
   		
		System.out.println("Testing http, https, ftp URLs...");
		UrlValidator validator = new UrlValidator();

   		validTest = validator.isValid("http://www.google.com");
   		collector.checkThat("basic http URL should be valid",  validTest, CoreMatchers.equalTo(true));
   		
   		validTest = validator.isValid("https://www.google.com");
   		collector.checkThat("basic https URL should be valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = validator.isValid("ftp://path/to/file");
   		collector.checkThat("basic ftp URL should be valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = validator.isValid("http://www.g*g.com");
   		collector.checkThat("invalid char should cause invalid http URL",  validTest, CoreMatchers.equalTo(false));
   		
   		validTest = validator.isValid("https://www.g*g.com");
   		collector.checkThat("invalid char should cause invalid https URL", validTest, CoreMatchers.equalTo(false));
   		
   		validTest = validator.isValid("ftp://foo.b!r.com");
   		collector.checkThat("invalid char should case invalid ftp URL", validTest, CoreMatchers.equalTo(false));
  		
   		System.out.println("Manual testing with default schemes complete. See JUnit output for details.\n");

	}
	
	@Test
   	public void manualTestAllSchemes() {

		System.out.println("MANUAL TESTING: Allowing All Schemes");
		
   		boolean validTest = false;
   		UrlValidator valAllSchemes = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);

   		/*
   		 * Manual test, all schemes allowed
   		 * CGP DEBUGGING NOTE: line 318 in UrlValidator.java
	 	 *		UrlValidator.java written such that "http" cannot allow trailing ':' in authority.
	 	 *		this should only be true for "file" schemes.
		 */
   		
   		// non-http protocols seem to be an issue, esp. with ALLOW_ALL_SCHEMES flag set:
   		// wrap in try/catch blocks to catch errors without terminating program

   		System.out.println("Testing with all schemes allowed...");
   		   		
   		validTest = valAllSchemes.isValid("http://www.google.com");
   		collector.checkThat("basic http URL should be valid",  validTest, CoreMatchers.equalTo(true));
   		
   		try {
   			validTest = valAllSchemes.isValid("https://www.google.com");
   			collector.checkThat("basic https URL should be valid", validTest, CoreMatchers.equalTo(true));
   		} catch (Throwable err) {
   			collector.addError(err);
   		}
   		
   		try {	
   	   		validTest = valAllSchemes.isValid("ftp://foo.bar.com");
   	   		collector.checkThat("basic ftp URL should be valid", validTest, CoreMatchers.equalTo(true));	
   		} catch (Throwable err) {
   			collector.addError(err);
   		}

   		try {
   			validTest = valAllSchemes.isValid("file://path/to/file");
   			collector.checkThat("basic file URL should be valid", validTest, CoreMatchers.equalTo(true));
   		} catch (Throwable err) {
   			collector.addError(err);
   		}
   		
   		try {	
   	   		validTest = valAllSchemes.isValid("h3t://url.net");
   	   		collector.checkThat("basic h3t URL should be valid", validTest, CoreMatchers.equalTo(true));	
   		} catch (Throwable err) {
   			collector.addError(err);
   		}	

   		try {	
   	   		validTest = valAllSchemes.isValid("3ht://url.net");
   	   		collector.checkThat("scheme starting with number should be invalid", validTest, CoreMatchers.equalTo(false));	
   		} catch (Throwable err) {
   			collector.addError(err);
   		}	
   		
   		// port: should be positive, integer
   		
   		validTest = valAllSchemes.isValid("http://www.google.com:65545");
   		collector.checkThat("URL with port num should be valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = valAllSchemes.isValid("http://www.google.com:-1");
   		collector.checkThat("negative port num should be invalid", validTest, CoreMatchers.equalTo(true));
   		
   		// ip address should be in range
   		
   		validTest = valAllSchemes.isValid("255.255.255.255");
   		collector.checkThat("IP address should be valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = valAllSchemes.isValid("http://256.256.256.256");
   		collector.checkThat("out-of-range IP addr. should be invalid", validTest, CoreMatchers.equalTo(false));
   		
   		// only alphanumeric chars in domain/authority
   		
   		validTest = valAllSchemes.isValid("http://www.g!.com");
   		collector.checkThat("non-alphanumeric chars in URL should be invalid", validTest, CoreMatchers.equalTo(false));
   	
   		validTest = valAllSchemes.isValid("http://www.google.c!m");
   		collector.checkThat("non-alphanumeric chars in URL should be invalid", validTest, CoreMatchers.equalTo(false));
   		
   		// null parameters should produce invalid result
   		
   		validTest = valAllSchemes.isValid(null);
   		collector.checkThat("NULL URL should be invalid",  validTest, CoreMatchers.equalTo(false));		
   		
   		// non-URLs should produce invalid result
   		
   		validTest = valAllSchemes.isValid("thisisnotaURL");
   		collector.checkThat("non-URL string should be invalid", validTest, CoreMatchers.equalTo(false));
	   	
   		// test paths	

   		validTest = valAllSchemes.isValid("http://www.google.com/path/to/the/file.html");
   		collector.checkThat("directory path in URL should be valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = valAllSchemes.isValid("http://www.google.com/..");
   		collector.checkThat("upper directory in URL path should be invalid", validTest, CoreMatchers.equalTo(false));

   		// query strings
   		
   		validTest = valAllSchemes.isValid("http://www.google.com?foo=bar&baz=bam");
   		collector.checkThat("query string should be recognized as valid", validTest, CoreMatchers.equalTo(true));
   		
   		validTest = valAllSchemes.isValid("http://www.google.com?");
   		collector.checkThat("empty query string (trailing \'?\') should be valid", validTest, CoreMatchers.equalTo(true)); 		
   		
   		// should be acceptable to eliminate scheme
   		
   		validTest = valAllSchemes.isValid("google.com");
   		collector.checkThat("no scheme in URL should be allowed/valid", validTest, CoreMatchers.equalTo(false));

   		try {
	   		validTest = valAllSchemes.isValid("ftp://...");
	   		collector.checkThat("badly-formed authority should be invalid", validTest, CoreMatchers.equalTo(false));
   		} catch (Throwable err) {
   			collector.addError(err);
   		}

   		System.out.println("Manual testing allowing all schemes complete. See JUnit output for details.\n");
 
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
