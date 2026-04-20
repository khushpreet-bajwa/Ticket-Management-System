function setOutput(text) {
  document.getElementById("output").textContent = text;
}

function setMinDate() {
  const journeyDateInput = document.getElementById("journeyDate");
  if (!journeyDateInput) return;

  const today = new Date();
  const year = today.getFullYear();
  const month = String(today.getMonth() + 1).padStart(2, "0");
  const day = String(today.getDate()).padStart(2, "0");
  const minDate = `${year}-${month}-${day}`;

  journeyDateInput.min = minDate;
}

function validateForm(name, source, destination, journeyDate) {
  if (!name || !source || !destination || !journeyDate) {
    setOutput("Please fill all fields.");
    return false;
  }

  if (source === destination) {
    setOutput("Source and destination cannot be same.");
    return false;
  }

  return true;
}

function clearBookingForm() {
  document.getElementById("name").value = "";
  document.getElementById("source").value = "";
  document.getElementById("destination").value = "";
  document.getElementById("journeyDate").value = "";
}

function extractValue(text, label) {
  const regex = new RegExp(`${label}\\s*:\\s*(.+)`, "i");
  const match = text.match(regex);
  return match ? match[1].trim() : "N/A";
}

function bookTicket() {
  const name = document.getElementById("name").value.trim();
  const source = document.getElementById("source").value;
  const destination = document.getElementById("destination").value;
  const journeyDate = document.getElementById("journeyDate").value;

  if (!validateForm(name, source, destination, journeyDate)) return;

  fetch(
    `/book?name=${encodeURIComponent(name)}&source=${encodeURIComponent(source)}&destination=${encodeURIComponent(destination)}&date=${encodeURIComponent(journeyDate)}`
  )
    .then((res) => res.text())
    .then((data) => {
      setOutput(data);

      if (
        data.includes("Ticket booked successfully") ||
        data.includes("Ticket Confirmed")
      ) {
        const ticketId = extractValue(data, "Ticket ID");
        const seatNo = extractValue(data, "Seat No");

        showConfirmedTicket(ticketId, name, source, destination, journeyDate, seatNo);
        clearBookingForm();
        setMinDate();
      }
    })
    .catch(() => setOutput("Error while booking ticket."));
}

function showConfirmedTicket(ticketId, name, source, destination, journeyDate, seatNo) {
  const ticketContainer = document.getElementById("ticketContainer");

  ticketContainer.innerHTML = `
    <div class="ticket-card">
      <div class="ticket-top">
        <h3>Confirmed Ticket</h3>
        <div class="ticket-status">CONFIRMED</div>
      </div>

      <div class="ticket-body">
        <div class="ticket-grid">
          <div class="ticket-field">
            <span>Ticket ID</span>
            <strong>${ticketId}</strong>
          </div>

          <div class="ticket-field">
            <span>Passenger Name</span>
            <strong>${name}</strong>
          </div>

          <div class="ticket-route">
            <div class="route-city">
              <span>Source</span>
              <strong>${source}</strong>
            </div>
            <div class="route-arrow">→</div>
            <div class="route-city">
              <span>Destination</span>
              <strong>${destination}</strong>
            </div>
          </div>

          <div class="ticket-field">
            <span>Journey Date</span>
            <strong>${journeyDate}</strong>
          </div>

          <div class="ticket-field">
            <span>Seat No</span>
            <strong>${seatNo}</strong>
          </div>
        </div>
      </div>
    </div>
  `;
}

function cancelTicket() {
  const id = document.getElementById("cancelId").value.trim();

  if (!id) {
    setOutput("Please enter ticket ID to cancel.");
    return;
  }

  fetch(`/cancel?id=${encodeURIComponent(id)}`)
    .then((res) => res.text())
    .then((data) => {
      setOutput(data);

      if (data.toLowerCase().includes("cancelled")) {
        document.getElementById("ticketContainer").innerHTML = "";
      }

      document.getElementById("cancelId").value = "";
    })
    .catch(() => setOutput("Error while cancelling ticket."));
}

function undoCancel() {
  fetch(`/undo`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while restoring ticket."));
}

function showTickets() {
  fetch(`/tickets`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading booked tickets."));
}

function showWaiting() {
  fetch(`/waiting`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading waiting list."));
}

function showHistory() {
  fetch(`/history`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading cancel history."));
}

function showSeats() {
  const journeyDate = document.getElementById("journeyDate").value;

  if (!journeyDate) {
    setOutput("Please select journey date first.");
    return;
  }

  fetch(`/seats?date=${encodeURIComponent(journeyDate)}`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading seats."));
}

window.addEventListener("DOMContentLoaded", () => {
  setMinDate();
});
function setMinDate() {
  const journeyDateInput = document.getElementById("journeyDate");
  if (!journeyDateInput) return;

  const today = new Date();
  const year = today.getFullYear();
  const month = String(today.getMonth() + 1).padStart(2, "0");
  const day = String(today.getDate()).padStart(2, "0");
  const minDate = `${year}-${month}-${day}`;

  journeyDateInput.min = minDate;
}

function validateForm(name, source, destination, journeyDate) {
  if (!name || !source || !destination || !journeyDate) {
    setOutput("Please fill all fields.");
    return false;
  }

  if (source === destination) {
    setOutput("Source and destination cannot be same.");
    return false;
  }

  return true;
}

function clearBookingForm() {
  document.getElementById("name").value = "";
  document.getElementById("source").value = "";
  document.getElementById("destination").value = "";
  document.getElementById("journeyDate").value = "";
}

function extractValue(text, label) {
  const regex = new RegExp(`${label}\\s*:\\s*(.+)`, "i");
  const match = text.match(regex);
  return match ? match[1].trim() : "N/A";
}

function bookTicket() {
  const name = document.getElementById("name").value.trim();
  const source = document.getElementById("source").value;
  const destination = document.getElementById("destination").value;
  const journeyDate = document.getElementById("journeyDate").value;

  if (!validateForm(name, source, destination, journeyDate)) return;

  fetch(
    `/book?name=${encodeURIComponent(name)}&source=${encodeURIComponent(source)}&destination=${encodeURIComponent(destination)}&date=${encodeURIComponent(journeyDate)}`
  )
    .then((res) => res.text())
    .then((data) => {
      setOutput(data);

      if (
        data.includes("Ticket booked successfully") ||
        data.includes("Ticket Confirmed")
      ) {
        const ticketId = extractValue(data, "Ticket ID");
        const seatNo = extractValue(data, "Seat No");

        showConfirmedTicket(ticketId, name, source, destination, journeyDate, seatNo);
        clearBookingForm();
        setMinDate();
      }
    })
    .catch(() => setOutput("Error while booking ticket."));
}

function showConfirmedTicket(ticketId, name, source, destination, journeyDate, seatNo) {
  const ticketContainer = document.getElementById("ticketContainer");

  ticketContainer.innerHTML = `
    <div class="ticket-card">
      <div class="ticket-top">
        <h3>Confirmed Ticket</h3>
        <div class="ticket-status">CONFIRMED</div>
      </div>

      <div class="ticket-body">
        <div class="ticket-grid">
          <div class="ticket-field">
            <span>Ticket ID</span>
            <strong>${ticketId}</strong>
          </div>

          <div class="ticket-field">
            <span>Passenger Name</span>
            <strong>${name}</strong>
          </div>

          <div class="ticket-route">
            <div class="route-city">
              <span>Source</span>
              <strong>${source}</strong>
            </div>
            <div class="route-arrow">→</div>
            <div class="route-city">
              <span>Destination</span>
              <strong>${destination}</strong>
            </div>
          </div>

          <div class="ticket-field">
            <span>Journey Date</span>
            <strong>${journeyDate}</strong>
          </div>

          <div class="ticket-field">
            <span>Seat No</span>
            <strong>${seatNo}</strong>
          </div>
        </div>
      </div>
    </div>
  `;
}

function cancelTicket() {
  const id = document.getElementById("cancelId").value.trim();

  if (!id) {
    setOutput("Please enter ticket ID to cancel.");
    return;
  }

  fetch(`/cancel?id=${encodeURIComponent(id)}`)
    .then((res) => res.text())
    .then((data) => {
      setOutput(data);

      if (data.toLowerCase().includes("cancelled")) {
        document.getElementById("ticketContainer").innerHTML = "";
      }

      document.getElementById("cancelId").value = "";
    })
    .catch(() => setOutput("Error while cancelling ticket."));
}

function undoCancel() {
  fetch(`/undo`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while restoring ticket."));
}

function showTickets() {
  fetch(`/tickets`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading booked tickets."));
}

function showWaiting() {
  fetch(`/waiting`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading waiting list."));
}

function showHistory() {
  fetch(`/history`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading cancel history."));
}

function showSeats() {
  fetch(`/seats`)
    .then((res) => res.text())
    .then((data) => setOutput(data))
    .catch(() => setOutput("Error while loading seats."));
}

window.addEventListener("DOMContentLoaded", () => {
  setMinDate();
});
