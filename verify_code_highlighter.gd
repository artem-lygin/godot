@tool
extends SceneTree

func _init():
	print("Running CodeHighlighter Font Style Verify...")
	
	var highlighter = CodeHighlighter.new()
	var pass_count = 0
	var fail_count = 0
	
	# Test 1: Add Keyword Style
	print("Test 1: Add Keyword Style")
	highlighter.add_keyword_color("test_keyword", Color.RED)
	highlighter.add_keyword_style("test_keyword", 1) # Bold
	
	if highlighter.get_keyword_style("test_keyword") == 1:
		print("PASS: Keyword style set correctly.")
		pass_count += 1
	else:
		print("FAIL: Keyword style mismatch. Expected 1, got ", highlighter.get_keyword_style("test_keyword"))
		fail_count += 1
		
	if highlighter.has_keyword_style("test_keyword"):
		print("PASS: has_keyword_style returned true.")
		pass_count += 1
	else:
		print("FAIL: has_keyword_style returned false.")
		fail_count += 1
		
	# Test 2: Add Member Keyword Style
	print("Test 2: Add Member Keyword Style")
	highlighter.add_member_keyword_color("member_keyword", Color.BLUE)
	highlighter.add_member_keyword_style("member_keyword", 2) # Italic
	
	if highlighter.get_member_keyword_style("member_keyword") == 2:
		print("PASS: Member keyword style set correctly.")
		pass_count += 1
	else:
		print("FAIL: Member keyword style mismatch. Expected 2, got ", highlighter.get_member_keyword_style("member_keyword"))
		fail_count += 1
		
	# Test 3: Color Regions with Style
	print("Test 3: Color Regions with Style")
	highlighter.add_color_region("\"", "\"", Color.GREEN, false, 3) # Bold + Italic
	
	# We can't easily get back the region style through public API as nicely as keywords (get_color_regions returns dict of colors)
	# But we can verify it doesn't crash and potentially check internal state if exposed (it's not).
	# However, we can check basic existence.
	if highlighter.has_color_region("\""):
		print("PASS: Color region added successfully.")
		pass_count += 1
	else:
		print("FAIL: Color region not found.")
		fail_count += 1

	# Test 4: Removal
	print("Test 4: Removal")
	highlighter.remove_keyword_style("test_keyword")
	if not highlighter.has_keyword_style("test_keyword"):
		print("PASS: Keyword style removed.")
		pass_count += 1
	else:
		print("FAIL: Keyword style still exists after removal.")
		fail_count += 1
		
	print("---------------------------------------------------")
	print("Tests Completed: ", pass_count, " Passed, ", fail_count, " Failed.")
	
	if fail_count == 0:
		print("VERIFICATION SUCCESSFUL")
		quit(0)
	else:
		print("VERIFICATION FAILED")
		quit(1)
