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
import java.util.Random; 

public class UrlValidatorTest {
	
	boolean PRINT_FAILURES = true;
	
	@Rule
	public ErrorCollector collector = new ErrorCollector();

	@Test
	public void testTestTuples() {
		
   		System.out.println("MANUAL TESTING: TestTuple Tests");

		// TestTuple is broken...see TestTuple.java in src dir.
		TestTuple rp_true = new TestTuple("http://www.google.com", true);
		TestTuple rp_false = new TestTuple("foo", false);
				
		System.out.println("Testing TestTuple objects...");
		collector.checkThat("TestTuple reporting incorrect valid property", rp_true.valid, CoreMatchers.equalTo(true));
		collector.checkThat("TestTuple reporting incorrect valid property", rp_false.valid, CoreMatchers.equalTo(false));
		System.out.println("TestTuple testing complete. See JUnit output for details.\n");
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
		   // https://tools.ietf.org/html/rfc3986#section-3
		   int failedTests = 0;
		   int trials = 500;
		   boolean expectedResult = false; 
		   boolean actualResult = false;
		   String urlGenerated;
		   TestTuple[] testUrl = new TestTuple[5];
		   Random rand = new Random(); 
		   System.out.println("_________________________________________________________");
		   System.out.println("             BEGINNING RANDOMIZED UNIT TEST              ");
		   // combine from the testtuples and test combinations
		   for(int i = 0; i < trials; i++) {
			   // get random scheme
			   testUrl[0] = testScheme[rand.nextInt(testScheme.length)];
			   // get random authority
			   testUrl[1] = testAuthority[rand.nextInt(testAuthority.length)];
			   // get random port
			   testUrl[2] = testPort[rand.nextInt(testPort.length)];
			   // get random path
			   testUrl[3] = testPath[rand.nextInt(testPath.length)];
			   // get random query
			   testUrl[4] = testQuery[rand.nextInt(testQuery.length)];
			   
			   // test tuple values of all - if all true, then 
			   // should pass else fail
			   expectedResult = testUrl[0].getValid() && testUrl[2].getValid() &&
					   testUrl[3].getValid() && testUrl[4].getValid();
			   
			   // run url validator on the generated url
			   urlGenerated = testUrl[0].getItem() + testUrl[2].getItem() +
					   testUrl[3].getItem() + testUrl[4].getItem();
			   UrlValidator validator = new UrlValidator(UrlValidator.ALLOW_ALL_SCHEMES);
			   try {
				   actualResult = validator.isValid(urlGenerated);
			   } catch (Throwable err) {
				   collector.addError(err);
			   }

			   // test if url validator returns the same value
			   // as the test tuples (control value)
			   collector.checkThat(validator.isValid(urlGenerated), CoreMatchers.equalTo(expectedResult));
			   // log results to console
			   if (expectedResult != actualResult) {
				   failedTests++;
				   System.out.println("--FAILURE: " + urlGenerated + "-> expected: "
						   + expectedResult + " actual: " + actualResult);
			   }
		   }
		   // log final results to console
		   System.out.println("_________________________________________________________");
		   System.out.println("             RANDOMIZED UNIT TEST RESULTS ");
		   System.out.println(failedTests + " failures out of " + trials);
		   System.out.println("_________________________________________________________"); 
	   }
	   
	   // Most of these test cases were borrowed from the Apache commons
	   // URL validator.
	   
	   // Scheme names consist of a sequence of characters beginning with a
	   // letter and followed by any combination of letters, digits, plus
	   // ("+"), period ("."), or hyphen ("-").  Although schemes are case-
	   // insensitive, the canonical form is lowercase and documents that
	   // specify schemes must do so with lowercase letters.  An implementation
	   // should accept uppercase letters as equivalent to lowercase in scheme
	   // names (e.g., allow "HTTP" as well as "http") for the sake of
	   // robustness but should only produce lowercase scheme names for
	   // consistency.	   
	   TestTuple[] testScheme = {
               new TestTuple("http://", true),
               new TestTuple("ftp://", true),
               new TestTuple("h3t://", true),
               new TestTuple("3ht://", false),
               new TestTuple("http:/", false),
               new TestTuple("http:", false),
               new TestTuple("http/", false),
               new TestTuple("://", false),
               new TestTuple("", true)
	   };
	   
	   // The authority component is preceded by a double slash ("//") and is
	   // terminated by the next slash ("/"), question mark ("?"), or number
	   // sign ("#") character, or by the end of the URI.
	   TestTuple[] testAuthority = {
               new TestTuple("www.google.com", true),
               new TestTuple("go.com", true),
               new TestTuple("go.au", true),
               new TestTuple("0.0.0.0", true),
               new TestTuple("255.255.255.255", true),
               new TestTuple("256.256.256.256", false),
               new TestTuple("255.com", true),
               new TestTuple("1.2.3.4.5", false),
               new TestTuple("1.2.3.4.", false),
               new TestTuple("1.2.3", false),
               new TestTuple(".1.2.3.4", false),
               new TestTuple("go.a", false),
               new TestTuple("go.a1a", false),
               new TestTuple("go.1aa", false),
               new TestTuple("aaa.", false),
               new TestTuple(".aaa", false),
               new TestTuple("aaa", false),
               new TestTuple("", false)
	   };
	   
	   // The port subcomponent of authority is designated by an optional port
	   // number in decimal following the host and delimited from it by a
	   // single colon (":") character.
	   TestTuple[] testPort = {
               new TestTuple(":80", true),
               new TestTuple(":65535", true),
               new TestTuple(":0", true),
               new TestTuple("", true),
               new TestTuple(":-1", false),
               new TestTuple(":65636",false),
               new TestTuple(":65a", false)
	   };
	   
	   // The path is terminated by the first question mark
	   //  ("?") or number sign ("#") character, or by the end
	   //  of the URI.
	   TestTuple[] testPath = {
               new TestTuple("/test1", true),
               new TestTuple("/t123", true),
               new TestTuple("/$23", true),
               new TestTuple("/..", false),
               new TestTuple("/../", false),
               new TestTuple("/test1/", true),
               new TestTuple("", true),
               new TestTuple("/test1/file", true),
               new TestTuple("/..//file", false),
               new TestTuple("/test1//file", false)
	   };
	   
	   // The query component is indicated by the first question
	   // mark ("?") character and terminated by a number sign ("#") character
	   // or by the end of the URI.
	   TestTuple[] testQuery = {
               new TestTuple("?action=view", true),
               new TestTuple("?action=edit&mode=up", true),
               new TestTuple("", true),
               new TestTuple("#", true),
               new TestTuple("&action=view", false)
	   };
	   
}
