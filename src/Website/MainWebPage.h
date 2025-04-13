#ifndef mw_HTML_H
#define mw_HTML_H
const char MainWebPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>System Scheduler | Dashboard</title>
    <script src="/jQuery.js"></script>
    <style>
        /* Base Styles */
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        }

        body {
            background-color: #f8f9fa;
            color: #333;
        }

        /* Header Styles */
        .header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            background-color: #2ecc71;
            padding: 12px 20px;
            color: white;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
        }

        .logo-container {
            display: flex;
            align-items: center;
            cursor: pointer;
        }

        .logo-svg {
            height: 40px;
            width: auto;
        }

        .action-buttons {
            display: flex;
            gap: 15px;
        }

        .icon-button {
            background: rgba(255, 255, 255, 0.2);
            border: none;
            color: white;
            width: 40px;
            height: 40px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            transition: all 0.3s;
        }

        .icon-button:hover {
            background: rgba(255, 255, 255, 0.3);
            transform: scale(1.05);
        }

        .icon-button svg {
            width: 24px;
            height: 24px;
        }

        /* Table Styles */
        .schedule-table {
            width: 100%;
            border-collapse: collapse;
            margin-top: 20px;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.05);
            background: white;
            border-radius: 8px;
            overflow: hidden;
        }

        .schedule-table th,
        .schedule-table td {
            padding: 15px;
            text-align: center;
            border-bottom: 1px solid #eee;
        }

        .schedule-table th {
            background-color: #f1f1f1;
            font-weight: 600;
            color: #555;
        }

        .schedule-table tr:hover {
            background-color: #f9f9f9;
        }

        .delete-button {
            cursor: pointer;
            color: #e74c3c;
        }

        /* Container Styles */
        .main-container {
            padding: 20px;
            max-width: 1200px;
            margin: 0 auto;
        }

        .date-info {
            margin-bottom: 20px;
            color: #555;
        }

        /* Modal Styles */
        .modal-overlay {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            right: 0;
            bottom: 0;
            background-color: rgba(0, 0, 0, 0.5);
            z-index: 1000;
        }

        .modal-content {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background-color: white;
            padding: 25px;
            border-radius: 8px;
            width: 90%;
            max-width: 500px;
            max-height: 90vh;
            overflow-y: auto;
            box-shadow: 0 5px 30px rgba(0, 0, 0, 0.3);
        }

        .modal-close-button {
            position: absolute;
            top: 15px;
            right: 15px;
            font-size: 20px;
            cursor: pointer;
            color: #777;
        }

        /* Form Styles */
        .form-group {
            margin-bottom: 15px;
        }

        .form-label {
            display: block;
            margin-bottom: 5px;
            font-weight: 500;
            color: #555;
        }

        .form-input {
            width: 100%;
            padding: 10px;
            border: 1px solid #ddd;
            border-radius: 4px;
            font-size: 14px;
        }

        .form-row {
            display: flex;
            gap: 10px;
        }

        .form-row .form-group {
            flex: 1;
        }

        .primary-button {
            background-color: #2ecc71;
            color: white;
            border: none;
            padding: 10px 15px;
            border-radius: 4px;
            cursor: pointer;
            font-weight: 500;
            width: 100%;
            margin-top: 10px;
        }

        .days-checkbox-container {
            display: flex;
            gap: 10px;
            flex-wrap: wrap;
        }

        .day-checkbox-label {
            display: flex;
            align-items: center;
            gap: 5px;
        }

        /* Badge Styles */
        .badge {
            padding: 5px 8px;
            border-radius: 4px;
            font-size: 12px;
            font-weight: 500;
            color: white;
        }

        .badge-tone {
            background-color: #3498db;
        }

        .badge-bell {
            background-color: #e67e22;
        }

        .badge-music {
            background-color: #9b59b6;
        }

        .badge-output {
            background-color: #16a085;
        }

        /* Validation Styles */
        .error-message {
            color: #e74c3c;
            font-size: 12px;
            margin-top: 5px;
            display: none;
        }

        .form-input.error {
            border-color: #e74c3c;
        }

        .toggle-container {
            display: flex;
            flex-wrap: wrap;
            gap: 12px;
        }

        .toggle-item {
            flex: 1;
            min-width: 120px;
            background: #f9f9f9;
            border-radius: 6px;
            padding: 12px;
            box-shadow: 0 1px 3px rgba(0, 0, 0, 0.05);
        }

        .toggle-label {
            font-size: 12px;
            color: #666;
            margin-bottom: 5px;
        }

        .toggle-button {
            display: inline-block;
            position: relative;
            width: 60px;
            height: 30px;
            border-radius: 15px;
            background-color: #ccc;
            cursor: pointer;
            transition: background-color 0.3s;
        }

        .toggle-button.active {
            background-color: #4CAF50;
        }

        .toggle-button:before {
            content: "";
            position: absolute;
            width: 26px;
            height: 26px;
            border-radius: 50%;
            top: 2px;
            left: 2px;
            background-color: white;
            transition: transform 0.3s;
            box-shadow: 0 1px 3px rgba(0, 0, 0, 0.2);
        }

        .toggle-button.active:before {
            transform: translateX(30px);
        }

        .toggle-text {
            display: inline-block;
            margin-left: 10px;
            font-weight: 500;
        }
    </style>
</head>

<body>
    <!-- Header with improved logo -->
    <header class="header">
        <div class="logo-container" onclick="refreshScheduleData()">
            <svg class="logo-svg" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 270 40">
                <defs>
                    <filter id="glow" x="-20%" y="-20%" width="140%" height="140%">
                        <feGaussianBlur stdDeviation="2" result="blur" />
                        <feComposite in="SourceGraphic" in2="blur" operator="over" />
                    </filter>
                </defs>
                <!-- Clock icon -->
                <circle cx="15" cy="20" r="12" fill="none" stroke="white" stroke-width="2" />
                <line x1="15" y1="20" x2="15" y2="12" stroke="white" stroke-width="2" stroke-linecap="round" />
                <line x1="15" y1="20" x2="21" y2="20" stroke="white" stroke-width="2" stroke-linecap="round" />

                <!-- Text -->
                <text x="35" y="25" font-family="Arial, sans-serif" font-size="20" font-weight="bold" fill="white"
                    filter="url(#glow)">System Scheduler</text>
            </svg>
        </div>

        <div class="action-buttons">
            <!-- Update button -->
            <div class="icon-button" onclick="updateScheduleData()">
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                        d="M4 4v5h.582m15.356 2A8.001 8.001 0 004.582 9m0 0H9m11 11v-5h-.581m0 0a8.003 8.003 0 01-15.357-2m15.357 2H15" />
                </svg>
            </div>

            <!-- Add schedule button -->
            <div class="icon-button" onclick="openAddScheduleModal()">
                <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                    <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                        d="M12 6v6m0 0v6m0-6h6m-6 0H6" />
                </svg>
            </div>

            <!-- Settings button -->
            <div class="icon-button"><a href="/Configuration">
                    <svg xmlns="http://www.w3.org/2000/svg" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                            d="M10.325 4.317c.426-1.756 2.924-1.756 3.35 0a1.724 1.724 0 002.573 1.066c1.543-.94 3.31.826 2.37 2.37a1.724 1.724 0 001.065 2.572c1.756.426 1.756 2.924 0 3.35a1.724 1.724 0 00-1.066 2.573c.94 1.543-.826 3.31-2.37 2.37a1.724 1.724 0 00-2.572 1.065c-.426 1.756-2.924 1.756-3.35 0a1.724 1.724 0 00-2.573-1.066c-1.543.94-3.31-.826-2.37-2.37a1.724 1.724 0 00-1.065-2.572c-1.756-.426-1.756-2.924 0-3.35a1.724 1.724 0 001.066-2.573c-.94-1.543.826-3.31 2.37-2.37.996.608 2.296.07 2.572-1.065z" />
                        <path stroke-linecap="round" stroke-linejoin="round" stroke-width="2"
                            d="M15 12a3 3 0 11-6 0 3 3 0 016 0z" />
                    </svg></a>
            </div>
        </div>
    </header>

    <!-- Add Schedule Modal -->
    <div id="addScheduleModal" class="modal-overlay">
        <div class="modal-content">
            <span class="modal-close-button" onclick="closeAddScheduleModal()">&times;</span>
            <h2>Add New Schedule</h2>
            <form id="scheduleForm">
                <div class="form-group">
                    <label for="scheduleTitle" class="form-label">Message/Title</label>
                    <input id="scheduleTitle" name="pesan" type="text" class="form-input"
                        placeholder="Enter schedule title">
                </div>

                <div class="form-row">
                    <div class="form-group">
                        <label for="scheduleDate" class="form-label required-label">Date</label>
                        <input id="scheduleDate" name="Date" type="date" class="form-input" required>
                        <div class="error-message" id="scheduleDate-error">Date is required</div>
                    </div>
                    <div class="form-group">
                        <label for="scheduleTime" class="form-label required-label">Time</label>
                        <input id="scheduleTime" name="Clock" type="time" class="form-input" required>
                        <div class="error-message" id="scheduleTime-error">Time is required</div>
                    </div>
                </div>

                <div class="form-group">
                    <label for="scheduleType" class="form-label">Schedule Type</label>
                    <select id="scheduleType" name="Tipe" class="form-input" onchange="handleScheduleTypeChange(this)">
                        <option value="1">Daily</option>
                        <option value="2">One Time</option>
                        <option value="3">By Day</option>
                        <option value="4">By Date</option>
                        <option value="5">By Time</option>
                    </select>
                </div>

                <div id="daysSelectionSection" class="form-group" style="display: none;">
                    <label class="form-label">Select Days</label>
                    <div class="days-checkbox-container">
                        <label class="day-checkbox-label"><input name="senin" type="checkbox"> Mon</label>
                        <label class="day-checkbox-label"><input name="selasa" type="checkbox"> Tue</label>
                        <label class="day-checkbox-label"><input name="rabu" type="checkbox"> Wed</label>
                        <label class="day-checkbox-label"><input name="kamis" type="checkbox"> Thu</label>
                        <label class="day-checkbox-label"><input name="jumat" type="checkbox"> Fri</label>
                        <label class="day-checkbox-label"><input name="sabtu" type="checkbox"> Sat</label>
                        <label class="day-checkbox-label"><input name="minggu" type="checkbox"> Sun</label>
                    </div>
                </div>

                <div class="form-group">
                    <label for="scheduleMode" class="form-label">Mode</label>
                    <select id="scheduleMode" name="Mode" class="form-input" onchange="handleScheduleModeChange(this)">
                        <option value="1">Tone</option>
                        <option value="2">Bell</option>
                        <option value="3">Music</option>
                        <option value="4">IO</option>
                    </select>
                </div>

                <!-- Tone Settings -->
                <div id="toneSettings" class="form-group">
                    <div class="form-row">
                        <div class="form-group">
                            <label for="frequency" class="form-label">Frequency (Hz)</label>
                            <input id="frequency" name="frekuensi" type="number" min="1" max="20000" value="1000"
                                class="form-input">
                        </div>
                        <div class="form-group">
                            <label for="duration" class="form-label">Duration (ms)</label>
                            <input id="duration" name="waktu" type="number" value="3600" class="form-input">
                        </div>
                    </div>
                </div>

                <!-- Bell Settings -->
                <div id="bellSettings" class="form-group" style="display: none;">
                    <label for="bellDelay" class="form-label">Delay (ms)</label>
                    <input id="bellDelay" name="delay" type="number" min="1" max="1000" value="1000" class="form-input">
                </div>

                <!-- Music Settings -->
                <div id="musicSettings" class="form-group" style="display: none;">
                    <label for="musicNumber" class="form-label">Music Number</label>
                    <input id="musicNumber" name="number" type="number" min="1" max="9999" value="2" class="form-input">
                </div>

                <!-- IO Settings -->
                <div id="ioSettings" class="form-group" style="display: none;">
                    <div class="form-row">
                        <div class="form-group">
                            <label for="outputChannel" class="form-label">Output Channel</label>
                            <select id="outputChannel" name="jalur" class="form-input">
                                <option value="1">1</option>
                                <option value="2">2</option>
                                <option value="3">3</option>
                                <option value="4">4</option>
                            </select>
                        </div>
                        <div class="form-group">
                            <label for="outputState" class="form-label">State</label>
                            <select id="outputState" name="value" class="form-input">
                                <option value="0">ON</option>
                                <option value="1">OFF</option>
                            </select>
                        </div>
                    </div>
                </div>

                <!-- Billing Settings -->
                <div id="billingSettings" class="form-group" style="display: none;">
                    <div class="form-row">
                        <div class="form-group">
                            <label for="billingHours" class="form-label">Hours</label>
                            <input id="billingHours" name="jam" type="number" min="0" max="23" value="0"
                                class="form-input">
                        </div>
                        <div class="form-group">
                            <label for="billingMinutes" class="form-label">Minutes</label>
                            <input id="billingMinutes" name="menit" type="number" min="0" max="59" value="0"
                                class="form-input">
                        </div>
                    </div>
                </div>

                <button type="button" class="primary-button" onclick="validateAndSubmitSchedule()">Save
                    Schedule</button>
            </form>
        </div>
    </div>

    <div class="main-container">
        <div class="date-info">
            <div class="current-date"></div>
            <div class="system-date"></div>
        </div>
        <div class="toggle-container">
            <div class="toggle-control">
              <span class="toggle-button" id="toggle1" data-status="0" onclick="OpenLinkBackground('OutputOne?Write=true','toggle1')"></span>
            </div>
            <div class="toggle-control">
              <span class="toggle-button active" id="toggle2" data-status="1" onclick="OpenLinkBackground('OutputTwo?Write=true','toggle2')"></span>
            </div>
            <div class="toggle-control">
              <span class="toggle-button" id="toggle3" data-status="0" onclick="OpenLinkBackground('OutputThree?Write=true','toggle3')"></span>
            </div>
            <div class="toggle-control">
              <span class="toggle-button active" id="toggle4" data-status="1" onclick="OpenLinkBackground('OutputFour?Write=true','toggle4')"></span>
            </div>
          </div>
       
        <table class="schedule-table">
            <thead>
                <tr>
                    <th>Queue</th>
                    <th>Date & Time</th>
                    <th>Schedule</th>
                    <th>Mode</th>
                    <th>Message</th>
                    <th>Time Left</th>
                    <th onclick="deleteSchedule(0)">Action</th>
                </tr>
            </thead>
            <tbody class="schedule-data"></tbody>
        </table>
    </div> </div>

    <script>
        function OpenLinkBackground(link, element) {
            $.get(link, function (data) {
                console.log(data);
                if (data == 1) {
                    document.getElementById(element).classList.add("active");
                } else {
                    document.getElementById(element).classList.remove("active");
                }
            });
        }
            
        let temporaryScheduleData;

        $(document).ready(function () {
            OpenLinkBackground('OutputOne?Read=true','toggle1');
            OpenLinkBackground('OutputTwo?Read=true','toggle2');
            OpenLinkBackground('OutputThree?Read=true','toggle3');
            OpenLinkBackground('OutputFour?Read=true','toggle4');
            refreshScheduleData();
            setInterval(refreshScheduleData, 60000);
            setInterval(() => updateScheduleCountdown("0"), 1000);

            // Display current date
            const now = new Date();
            $(".current-date").text("Today: " + now.toDateString());
        });

        function openAddScheduleModal() {
            refreshScheduleData();
            document.getElementById('addScheduleModal').style.display = 'block';
        }

        function closeAddScheduleModal() {
            document.getElementById('addScheduleModal').style.display = 'none';
        }

        function refreshScheduleData() {
            $.get("ReadScheduler", function (data) {
                console.log(data);
                if (data != "no data")
                    updateScheduleDisplay(data);
            });

            $.get("Date", function (data) {
                console.log(data);
                $(".system-date").text("System date: " + data);
            });
        }

        function deleteSchedule(scheduleId) {
            if (confirm("Are you sure you want to delete this schedule?")) {
                $.get(`Delete?delete=${scheduleId}`, refreshScheduleData);
            }
        }

        function updateScheduleData() {
            $.get("Update", refreshScheduleData);
        }

        function handleScheduleTypeChange(scheduleTypeSelect) {
            if (scheduleTypeSelect.selectedIndex == 0 || scheduleTypeSelect.selectedIndex == 1 ||
                scheduleTypeSelect.selectedIndex == 3 || scheduleTypeSelect.selectedIndex == 4) {
                $("#daysSelectionSection").slideUp("fast");
            }
            if (scheduleTypeSelect.selectedIndex == 2) {
                $("#daysSelectionSection").slideDown("fast");
            }
        }

        function handleScheduleModeChange(scheduleModeSelect) {
            // Hide all settings first
            $("#toneSettings, #bellSettings, #musicSettings, #ioSettings, #billingSettings").hide();

            // Show the appropriate settings
            switch (scheduleModeSelect.selectedIndex) {
                case 0: // Tone
                    $("#toneSettings").show();
                    break;
                case 1: // Bell
                    $("#bellSettings").show();
                    break;
                case 2: // Music
                    $("#musicSettings").show();
                    break;
                case 3: // IO
                    $("#ioSettings").show();
                    break;
                case 4: // Billing
                    $("#billingSettings").show();
                    break;
            }
        }

        function validateField(field) {
            const fieldId = field.id;
            const errorElement = document.getElementById(fieldId + '-error');

            if (field.required && !field.value) {
                field.classList.add('error');
                errorElement.style.display = 'block';
                return false;
            } else {
                field.classList.remove('error');
                errorElement.style.display = 'none';
                return true;
            }
        }

        function resetValidation() {
            // Reset all validation styles
            const fields = document.querySelectorAll('.form-input');
            fields.forEach(field => {
                field.classList.remove('error');
                const errorElement = document.getElementById(field.id + '-error');
                if (errorElement) {
                    errorElement.style.display = 'none';
                }
            });
        }

        function validateAndSubmitSchedule() {
            // Validate all required fields
            const dateField = document.getElementById('scheduleDate');
            const timeField = document.getElementById('scheduleTime');

            const isDateValid = validateField(dateField);
            const isTimeValid = validateField(timeField);

            // If all validations pass, submit the form
            if (isDateValid && isTimeValid) {
                saveSchedule();
            } else {
                // Focus the first invalid field
                if (!isDateValid) dateField.focus();
                else if (!isTimeValid) timeField.focus();
            }
        }

        function saveSchedule() {
            closeAddScheduleModal();
            $.get("AddScheduler/?" + $("#scheduleForm").serialize(), function (data, status) {
                console.log(data);
                console.log($("#scheduleForm").serialize());
            });
            refreshScheduleData();
        }

        function updateScheduleDisplay(data) {
            $(".schedule-data").empty();
            let scheduleNumber = 0;

            if (data !== "0") {
                temporaryScheduleData = data;
                const dataParts = temporaryScheduleData.split("^");
                const scheduleEntries = dataParts[1].split("$");

                console.log(dataParts);

                scheduleEntries.forEach(entry => {
                    const scheduleData = entry.split("~");
                    if (scheduleData[1] == "" || scheduleData[1] == undefined) return;

                    let dateString = scheduleData[1];
                    dateString = dateString.replace("|", " ").replace(/,/g, "-");
                    const scheduleDate = new Date(dateString);
                    const timeLeft = calculateTimeLeft(scheduleDate);
                    const modeDescription = getModeDescription(scheduleData);
                    const daysDescription = getDaysDescription(scheduleData);
                    const modeClass = getModeClass(parseInt(scheduleData[9]));

                    scheduleNumber++;

                    const row = `
                        <tr>
                            <td>${scheduleNumber}</td>
                            <td>${scheduleDate.toLocaleString()}</td>
                            <td>${daysDescription}</td>
                            <td><span class="badge ${modeClass}">${modeDescription}</span></td>
                            <td>${scheduleData[16]}</td>
                            <td class="countdown-${scheduleData[0]}">${timeLeft}</td>
                            <td><span class="delete-button" onclick="deleteSchedule(${scheduleNumber})">Delete</span></td>
                        </tr>
                    `;

                    $(".schedule-data").append(row);
                });
            }
        }

        function getModeClass(modeNumber) {
            switch (modeNumber) {
                case 1: return "badge-tone";
                case 2: return "badge-bell";
                case 3: return "badge-music";
                case 4: return "badge-output";
                default: return "";
            }
        }

        function calculateTimeLeft(targetDate) {
            const now = new Date();
            let timeDifference = targetDate - now;

            if (timeDifference < 0) return "Passed";

            let hours = Math.floor(timeDifference / 3600000);
            let minutes = Math.floor((timeDifference % 3600000) / 60000);
            let seconds = Math.floor((timeDifference % 60000) / 1000);

            minutes = minutes.toString().padStart(2, '0');
            seconds = seconds.toString().padStart(2, '0');

            let timeLeft = `${hours}:${minutes}:${seconds}`;

            if (hours > 24) {
                let days = Math.floor(hours / 24);
                hours %= 24;
                hours = hours.toString().padStart(2, '0');
                timeLeft = `${days}d ${hours}:${minutes}:${seconds}`;
            }

            return timeLeft;
        }

        function getModeDescription(scheduleData) {
            switch (parseInt(scheduleData[9])) {
                case 1:
                    return `Tone (${scheduleData[10]}Hz)`;
                case 2:
                    return `Bell`;
                case 3:
                    return `Music #${scheduleData[13]}`;
                case 4:
                    return `Output ${scheduleData[15] == 0 ? "ON" : "OFF"}`;
                case 5:
                    return `Billing`;
                default:
                    return '';
            }
        }

        function getDaysDescription(scheduleData) {
            const days = ["Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"];
            let selectedDays = days.filter((day, index) => scheduleData[index + 2] == 1).join(", ");
            return selectedDays || "Once";
        }

        function updateScheduleCountdown(data) {
            if (temporaryScheduleData !== "" && temporaryScheduleData !== undefined) {
                console.log(temporaryScheduleData);
                const scheduleEntries = temporaryScheduleData.split("^")[1].split("$");

                scheduleEntries.forEach(entry => {
                    const scheduleData = entry.split("~");
                    if (!scheduleData[1]) return;

                    const scheduleDate = new Date(scheduleData[1].replace("|", " ").replace(/,/g, "-"));
                    const timeLeft = calculateTimeLeft(scheduleDate);
                    $(".countdown-" + scheduleData[0]).text(timeLeft);
                });
            }
        }

        // Initialize with tone settings displayed
        document.addEventListener('DOMContentLoaded', function () {
            document.getElementById('toneSettings').style.display = 'block';
        });

        // Close modal when clicking outside of it
        window.onclick = function (event) {
            const modal = document.getElementById('addScheduleModal');
            if (event.target == modal) {
                closeAddScheduleModal();
            }
        }
    </script>
</body>

</html>

)=====";

const char AddScheduler[] PROGMEM = R"=====(
    <style>
        .modal-container {
            left: calc(50vw - 300px);
            right: calc(50vw - 300px);
            top: calc(50vh - 300px);
            bottom: calc(50vh - 300px);
            position: absolute;
        }

        .add-schedule-form {
            width: 600px;
            height: 1000px;
            text-align: center;
            font-size: 30px;
        }

        .form-input,
        .form-select,
        .form-title,
        .form-button {
            display: flex;
            align-items: center;
            justify-content: center;
            text-align: center;
            width: 400px;
            height: 45px;
            font-size: 15px;
            text-align: center;
            margin: 5px;
            border: 0px solid black;
            box-shadow: 0px 5px 20px 10px rgba(0, 0, 0, 0.1);
            background-color: white;
        }

        .form-title {
            font-size: 25px;
        }
    </style>

    <div class="modal-container">
        <div class="add-schedule-form">
            <form action="#" method="get">
                <div style="position: absolute; right: 0px;" onclick="closeModal()">
                    X
                </div>
                <div style="padding: 45px; display:flex; flex-direction:column; align-items: center; justify-content: center;">
                    <input name="pesan" type="text" class="form-input">
                    <div style="display: flex;">
                        <input name="Date" type="date" class="form-input" style="width: 195px;">
                        <input name="Clock" type="time" class="form-input" style="width: 195px;">
                    </div>
                    <div class="form-title">Schedule Type</div>
                    <select name="Tipe" class="form-select" onchange="handleScheduleTypeChange(this)">
                        <option value="1">Daily</option>
                        <option value="2">One Time</option>
                        <option value="3">By Day</option>
                        <option value="4">By Date</option>
                        <option value="5">By Time</option>
                    </select>
                    <div class="form-title days-section-title" style="display: none;">Days Selection</div>
                    <div style="display: none;" class="days-section">
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <input name="senin" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="selasa" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="rabu" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="kamis" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="jumat" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="sabtu" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                            <input name="minggu" type="checkbox" class="form-input" style="width: 25px; box-shadow: none;">
                        </div>
                    </div>
                    <div class="form-title">Schedule Mode</div>
                    <select name="Mode" class="form-select" onchange="handleScheduleModeChange(this)">
                        <option value="1">Tone</option>
                        <option value="2">Bell</option>
                        <option value="3">Music</option>
                        <option value="4">IO</option>
                        <option value="5">Billing</option>
                    </select>
                    <div style="display: none;" class="tone-settings">
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">Frequency</div>
                            <input name="frekuensi" class="form-input" type="number" min="1" max="20000" value="1000">
                        </div>
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">Duration</div>
                            <input name="waktu" class="form-input" type="text" value="3600">
                        </div>
                    </div>
                    <div style="display: none;" class="billing-settings">
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">Hours</div>
                            <input name="jam" class="form-input" type="number" min="1" max="20000" value="1000">
                        </div>
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">Minutes</div>
                            <input name="menit" class="form-input" type="number" value="3600">
                        </div>
                    </div>
                    <div style="display: none;" class="form-title bell-delay-title">Bell Delay</div>
                    <input style="display: none;" class="form-input bell-delay-input" name="delay" type="number" min="1" max="1000" value="1000">
                    <div style="display: none;" class="form-title music-number-title">Music Number</div>
                    <input style="display: none;" class="form-input music-number-input" name="number" type="number" min="1" max="9999" value="2">
                    <div style="display: none;" class="io-settings">
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">Channel</div>
                            <select class="form-select" name="jalur">
                                <option value="16">1</option>
                                <option value="14">2</option>
                                <option value="12">3</option>
                                <option value="13">4</option>
                            </select>
                        </div>
                        <div style="display: flex; background-color: rgb(255, 255, 255);" class="form-title">
                            <div class="form-title">State</div>
                            <select class="form-select" name="value">
                                <option value="0">ON</option>
                                <option value="1">OFF</option>
                            </select>
                        </div>
                    </div>
                </div>
            </form>
            <div style="display: flex; justify-content: center;align-items: center;">
                <button class="form-button" onclick="saveSchedule()">Send</button>
            </div>
        </div>
    </div>

<script>
    function handleScheduleTypeChange(scheduleTypeSelect) {
        if (scheduleTypeSelect.selectedIndex == 0 || scheduleTypeSelect.selectedIndex == 1 || 
            scheduleTypeSelect.selectedIndex == 3 || scheduleTypeSelect.selectedIndex == 4) {
            $(".days-section, .days-section-title").slideUp("slow");
        }
        if (scheduleTypeSelect.selectedIndex == 2) {
            $(".days-section, .days-section-title").slideDown("slow");
        }
    }

    function handleScheduleModeChange(scheduleModeSelect) {
        if (scheduleModeSelect.selectedIndex == 0) {
            $(".tone-settings").slideDown("slow");
            $(".bell-delay-title, .bell-delay-input").slideUp("slow");
            $(".music-number-title, .music-number-input").slideUp("slow");
            $(".io-settings").slideUp("slow");
            $(".billing-settings").slideUp("slow");
        }
        else if (scheduleModeSelect.selectedIndex == 1) {
            $(".tone-settings").slideUp("slow");
            $(".bell-delay-title, .bell-delay-input").slideDown("slow");
            $(".music-number-title, .music-number-input").slideUp("slow");
            $(".io-settings").slideUp("slow");
            $(".billing-settings").slideUp("slow");
        }
        else if (scheduleModeSelect.selectedIndex == 2) {
            $(".tone-settings").slideUp("slow");
            $(".bell-delay-title, .bell-delay-input").slideUp("slow");
            $(".music-number-title, .music-number-input").slideDown("slow");
            $(".io-settings").slideUp("slow");
            $(".billing-settings").slideUp("slow");
        }
        else if (scheduleModeSelect.selectedIndex == 3) {
            $(".tone-settings").slideUp("slow");
            $(".bell-delay-title, .bell-delay-input").slideUp("slow");
            $(".music-number-title, .music-number-input").slideUp("slow");
            $(".io-settings").slideDown("slow");
            $(".billing-settings").slideUp("slow");
        }
        else if (scheduleModeSelect.selectedIndex == 4) {
            $(".tone-settings").slideUp("slow");
            $(".bell-delay-title, .bell-delay-input").slideUp("slow");
            $(".music-number-title, .music-number-input").slideUp("slow");
            $(".io-settings").slideUp("slow");
            $(".billing-settings").slideDown("slow");
        }
    }

    function closeModal() {
        $(".modal-container").fadeOut(100);
        refreshScheduleData();
    }

    function saveSchedule() {
        $(".modal-container").fadeOut(100);
        $.get("AddScheduler/?" + $("form").serialize(), function(data, status) {
            console.log(data);
            console.log($("form").serialize());
        }); 
        refreshScheduleData();
    }
</script>
)=====";
#endif