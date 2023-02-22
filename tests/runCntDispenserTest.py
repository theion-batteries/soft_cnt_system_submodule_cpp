import subprocess
import os

# Get the directory containing the Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the relative path to another file

# Replace with the path to your test executable
test_executable = os.path.join(script_dir, "../../../../../build/Debug/", "cntTests.exe")

def test(test_name):
    test_args = [test_executable, "--gtest_filter=CntDispenserTest."+test_name]
    test_result = subprocess.run(test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if test_result.returncode == 0:
            print(test_name+" test passed!")
    else:
           print(test_name+" test failed:")
           print(test_result.stdout.decode())
           print(test_result.stderr.decode())





if __name__ == "__main__":
    tests= ["Connect", "IsConnected", "Activate","Vibrate","SetFreq","GetFreq","SetDuration","GetDuration"]

    for testName in tests:
        test(testName)  
