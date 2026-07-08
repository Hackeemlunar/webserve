// UI Elements
const select = document.getElementById('filePath');
const resultMessage = document.getElementById('resultMessage');
const showLoginBtn = document.getElementById('show-login-btn');
const logoutForm = document.getElementById('logout-form');
const userDisplay = document.getElementById('user-display');
const loginPanel = document.getElementById('login-panel');
const filePanel = document.getElementById('file-panel');
const panelTitle = document.getElementById('panel-title');
const cancelLoginBtn = document.getElementById('cancel-login');
const uploadButton = document.getElementById('upload-link');

// Tracks whichever folder is currently being browsed (public root or a
// specific user's folder). Needed so the delete handler knows where to
// refresh from, since that depends on session state.
let currentListPath = '/uploads/public';

// Toggle the login form
showLoginBtn.addEventListener('click', () => {
    loginPanel.classList.remove('hidden');
    filePanel.classList.add('hidden');
});

cancelLoginBtn.addEventListener('click', () => {
    loginPanel.classList.add('hidden');
    filePanel.classList.remove('hidden');
});

// Load files depending on session state (Clean async/await structure)
async function loadFileOptions(fetchUrl) {
    try {
        const res = await fetch(fetchUrl);
        const html = await res.text();
        const doc = new DOMParser().parseFromString(html, 'text/html');

        select.innerHTML = '';

        doc.querySelectorAll('a').forEach(a => {
            const href = decodeURIComponent(a.getAttribute('href'));
            const fileName = href.replace(/^.*\//, '');

            if (href && href !== '.' && href !== '..') {
                const opt = document.createElement('option');
                opt.value = href;
                opt.textContent = fileName;
                select.appendChild(opt);
            }
        });
    } catch (error) {
        console.error('Error loading files:', error);
    }
}

// Delete action (Async/await with template literals)
document.getElementById('deleteButton').addEventListener('click', async () => {
    const filePath = select.value;
    if (!filePath) return;

    try {
        const res = await fetch(encodeURI(filePath), { method: 'DELETE' });

        resultMessage.textContent = res.ok ? `Deleted "${filePath}"` : `Failed (status ${res.status})`;
        resultMessage.className = `result-message ${res.ok ? 'success' : 'error'}`;

        // Refresh whichever folder we're currently browsing, not a
        // hardcoded path.
        loadFileOptions(currentListPath);
    } catch (error) {
        resultMessage.textContent = 'Error deleting file';
        resultMessage.className = 'result-message error';
    }
});

// Initialize Session State
(async function initSession() {
    const fallbackToGlobal = () => {
        showLoginBtn.classList.remove('hidden');
        panelTitle.textContent = '🌍 Global Uploads';
        currentListPath = '/uploads/public';
        uploadButton.href = '/uploads/public';
        uploadButton.textContent = '🌍 Global Uploads';
        loadFileOptions(currentListPath);
    };

    try {
        const res = await fetch('/session');
        const text = await res.text();
        const prefix = 'Logged in as ';

        if (!text.startsWith(prefix)) {
            fallbackToGlobal();
            return;
        }

        // HAS SESSION: Show personal files & Logout button
        const username = text.slice(prefix.length).split('\n')[0];
        const userPath = `/uploads/user/${encodeURIComponent(username)}`;

        logoutForm.classList.remove('hidden');
        userDisplay.textContent = `(${username})`;
        panelTitle.textContent = '📁 Upload files';

        // Path now correctly includes the username segment.
        currentListPath = userPath;
        uploadButton.href = userPath;
        uploadButton.textContent = `📁 My Uploads (${username})`;

        loadFileOptions(currentListPath);
    } catch {
        fallbackToGlobal();
    }
})();