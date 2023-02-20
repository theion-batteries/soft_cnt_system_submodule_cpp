import subprocess
import os

# Get the directory containing the Python script
script_dir = os.path.dirname(os.path.abspath(__file__))

# Construct the relative path to another file

# Replace with the path to your test executable
test_executable = os.path.join(script_dir, "../../../../../build/Debug/", "cntTests.exe")
# Run the Connect test
connect_test_args = [test_executable, "--gtest_filter=CntDispenserTest.Connect"]
connect_test_result = subprocess.run(connect_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if connect_test_result.returncode == 0:
    print("Connect test passed!")
else:
    print("Connect test failed:")
    print(connect_test_result.stdout.decode())
    print(connect_test_result.stderr.decode())

# Run the IsConnected test
isconnected_test_args = [test_executable, "--gtest_filter=CntDispenserTest.IsConnected"]
isconnected_test_result = subprocess.run(isconnected_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the IsConnected test
if isconnected_test_result.returncode == 0:
    print("IsConnected test passed!")
else:
    print("IsConnected test failed:")
    print(isconnected_test_result.stdout.decode())
    print(isconnected_test_result.stderr.decode())



activate_test_args = [test_executable, "--gtest_filter=CntDispenserTest.Activate"]
activate_test_result = subprocess.run(activate_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if activate_test_result.returncode == 0:
    print("Activate test passed!")
else:
    print("Activate test failed:")
    print(activate_test_result.stdout.decode())
    print(activate_test_result.stderr.decode())


deactivate_test_args = [test_executable, "--gtest_filter=CntDispenserTest.Deactivate"]
deactivate_test_result = subprocess.run(deactivate_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if deactivate_test_result.returncode == 0:
    print("Deactivate test passed!")
else:
    print("Deactivate test failed:")
    print(deactivate_test_result.stdout.decode())
    print(deactivate_test_result.stderr.decode())



vibrate_test_args = [test_executable, "--gtest_filter=CntDispenserTest.Vibrate"]
vibrate_test_result = subprocess.run(deactivate_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if vibrate_test_result.returncode == 0:
    print("Vibrate test passed!")
else:
    print("Vibrate test failed:")
    print(vibrate_test_result.stdout.decode())
    print(vibrate_test_result.stderr.decode())


setFreq_test_args = [test_executable, "--gtest_filter=CntDispenserTest.SetFrequency"]
setFreq_test_result = subprocess.run(setFreq_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if setFreq_test_result.returncode == 0:
    print("SetFreq test passed!")
else:
    print("setFreq test failed:")
    print(setFreq_test_result.stdout.decode())
    print(setFreq_test_result.stderr.decode())



getFreq_test_args = [test_executable, "--gtest_filter=CntDispenserTest.GetFrequency"]
getFreq_test_result = subprocess.run(getFreq_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if getFreq_test_result.returncode == 0:
    print("GetFreq test passed!")
else:
    print("GetFreq test failed:")
    print(getFreq_test_result.stdout.decode())
    print(getFreq_test_result.stderr.decode())





setDur_test_args = [test_executable, "--gtest_filter=CntDispenserTest.SetDuration"]
setDur_test_result = subprocess.run(setFreq_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if setDur_test_result.returncode == 0:
    print("SetDuration test passed!")
else:
    print("SetDuration test failed:")
    print(setDur_test_result.stdout.decode())
    print(setDur_test_result.stderr.decode())


getDur_test_args = [test_executable, "--gtest_filter=CntDispenserTest.GetDuration"]
getDur_test_result = subprocess.run(setFreq_test_args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# Check the return code of the Connect test
if getDur_test_result.returncode == 0:
    print("GetDuration test passed!")
else:
    print("GetDuration test failed:")
    print(getDur_test_result.stdout.decode())
    print(getDur_test_result.stderr.decode())