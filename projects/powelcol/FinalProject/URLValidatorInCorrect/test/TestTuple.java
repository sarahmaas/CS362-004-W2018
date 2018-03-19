/*
 * CS362
 * Final Project: Part B
 * Sarah Maas (maass), Colin Powell (powelcol), Josh Williams (willijo4)
 * TestPair: groups a URL and its "validity" - used for URL unit tests
 * Based on ResultPair class found in UrlValidator source code.
 */

public class TestTuple {

	private boolean valid;
	private String item;
	private String purpose;
	
	public TestTuple(String purpose, String url, boolean valid) {
		this.purpose = purpose;
		this.item = url;
		this.valid = valid;
	}
	
	public TestTuple(String url, boolean valid) {
		this.item = url;
		this.valid = valid;
		this.purpose = "";
	}
	
	public String getPurpose() {
		return this.purpose;
	}
	
	public boolean getValid() {
		return this.valid;
	}
	
	public String getItem() {
		return this.item;
	}
	
}
