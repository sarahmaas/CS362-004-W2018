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
	 // Set a few valid URLs that should be valid, then test those

  	System.out.println("********First Partition: Valid URLs********");
		
	System.out.println("Testing valid URLs...");
	String[] schemes = { "http", "https", "ftp" };
	UrlValidator urlVal = new UrlValidator(schemes);
	
	// Test that the schemes are valid
	collector.checkThat("expect basic http-based URL to be valid", 
	  	urlVal.isValid("http://www.google.com"), CoreMatchers.equalTo(true));
	collector.checkThat("expect basic http-based URL to be valid", 
		urlVal.isValid("https://www.google.com"), CoreMatchers.equalTo(true));
	collector.checkThat("expect basic http-based URL to be valid", 
		urlVal.isValid("ftp://foo.bar.com/"), CoreMatchers.equalTo(true));
	collector.checkThat("expect basic http-based URL to be valid", 
			urlVal.isValid("h3t://foo.bar.com/"), CoreMatchers.equalTo(true));
	collector.checkThat("expect basic http-based URL to be valid", 
			urlVal.isValid("foo.bar.com/"), CoreMatchers.equalTo(true));

	
	urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	
	// Test several ports that are within range
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com"), CoreMatchers.equalTo(true));
   	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com:0"), CoreMatchers.equalTo(true));
   	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com:80"), CoreMatchers.equalTo(true));
   	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com:555"), CoreMatchers.equalTo(true));
   	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com:4000"), CoreMatchers.equalTo(true));
   	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Port # should be correct", urlVal.isValid("http://www.google.com:65535"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
		
	// Test valid authorities
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("http://www.google.com"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("go.com"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("go.au"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("0.0.0.0"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("255.255.255.255"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Authority should be correct", urlVal.isValid("255.com"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	
	// Test several valid paths
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/mail"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/maps"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/flights"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/test1"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/t123"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/$23"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/test1/"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/test1/file"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Path should be valid", urlVal.isValid("https://www.google.com/"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}

	// Test several valid queries
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("http://www.google.com?foo=bar&baz=bam"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("https://www.google.com/search?safe=off&source=hp&ei=7QavWqioM4nXjwSz2bmIBQ&q=who+there&oq=who+there&gs_l=psy-ab.3..0l3j0i10k1j0l6.42298.43886.0.44040.13.8.0.0.0.0.114.708.7j1.8.0....0...1c.1.64.psy-ab..5.8.708.0..46j0i131k1j0i46k1.0.xNFhvn885ic"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("http://www.google.com?name=John&city=NYC"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("http://www.google.com?action=view"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("http://www.google.com?action=edit&mode=up"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("Query should be valid", urlVal.isValid("http://www.google.com"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}

	// Test valid URL fragments
	try {
		collector.checkThat("URL fragment should be valid", urlVal.isValid("http://www.google.com/mail#two"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("URL fragment should be valid", urlVal.isValid("http://www.example.org/foo.html#bar"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
	try {
		collector.checkThat("URL fragment should be valid", urlVal.isValid("http://www.w3.org/2004/02/skos/core#broader"), CoreMatchers.equalTo(true));
	} catch (Throwable err) {
   		collector.addError(err);
   	}
 	
  	System.out.println("********First Partition Complete********\n");
		
   }
   
   @Test
   public void testYourSecondPartition() {
	  	System.out.println("********Second Partition: Invalid URLs********");
		
		System.out.println("Testing invalid URLs...");
		String[] schemes = { "http", "https", "ftp" };
		UrlValidator urlVal = new UrlValidator(schemes);

	  	// Test URLs with invalid scheme
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("httcs://www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("phtp://www.foo.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("h++p://www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("http:/www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("http:www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("http/www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("://www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("expect basic http-based URL to be valid", urlVal.isValid("3ht://www.google.com"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}

	  	urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	  	
	  	// Test URLs with invalid ports
		try {
			collector.checkThat("Testing with invalid port", urlVal.isValid("https://www.google.com:-11"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid port", urlVal.isValid("https://www.google.com:-1"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid port", urlVal.isValid("https://www.google.com:65536"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid port", urlVal.isValid("https://www.google.com:66666666"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid port", urlVal.isValid("https://www.google.com:65a"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
	   
	  	// Test URLs with invalid authorities
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("256.256.256.256"), CoreMatchers.equalTo(false));
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("1.2.3.4.5"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("1.2.3.4."), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("1.2.3"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid(".1.2.3.4"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("go.a"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("go.a1a"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("go.1aa"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("aaa."), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid(".aaa"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("aaa"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid authority", urlVal.isValid("http://"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
	  	
	  	// Test URLs with invalid path
		try {
			collector.checkThat("Testing with invalid path", urlVal.isValid("https://www.google.com/.."), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid path", urlVal.isValid("https://www.google.com/../"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid path", urlVal.isValid("https://www.google.com/..//file"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid path", urlVal.isValid("https://www.google.com/test1//file"), CoreMatchers.equalTo(false));   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
	   
	  	// Test URLs with invalid query
		try {
			collector.checkThat("Testing with invalid query", urlVal.isValid("http://www.google.com,foo=bar&baz=bam"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid query", urlVal.isValid("http://www.google.com$foo=bar&baz=bam"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid query", urlVal.isValid("http://www.google.com!action=yes"), CoreMatchers.equalTo(false));	   
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
	   
	  	// Test URLs with invalid fragment
		try {
			collector.checkThat("Testing with invalid fragment", urlVal.isValid("https://www.google.com%ll"), CoreMatchers.equalTo(false));
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid fragment", urlVal.isValid("https://www.google.comtest$###"), CoreMatchers.equalTo(false));
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
		try {
			collector.checkThat("Testing with invalid fragment", urlVal.isValid("https://www.google.com*("), CoreMatchers.equalTo(false));
		} catch (Throwable err) {
	   		collector.addError(err);
	   	}
	  	
	  	System.out.println("Second Partition Complete\n"); 
   }
	   
   @Test
   public void testIsValid() {
	   //You can use this function for programming based testing
	
   }
}
