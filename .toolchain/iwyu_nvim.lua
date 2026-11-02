local M = {}
local root = "/home/ubuntu/projects/cp"

local function append_once(list, value)
  if not vim.tbl_contains(list, value) then
    list[#list + 1] = value
  end
end

local function in_project(filename)
  return filename:sub(1, #root + 1) == root .. "/"
    and (filename:match("%.c$") or filename:match("%.h$"))
end

function M.parse(output, bufnr, _)
  local filename = vim.fs.normalize(vim.api.nvim_buf_get_name(bufnr))
  local diagnostics = {}
  local removing = false
  for line in output:gmatch("[^\r\n]+") do
    local file, row, col, message = line:match("^(.-):(%d+):(%d+): warning: (.*)$")
    if file and vim.fs.normalize(file) == filename
      and (message:find("which isn't directly #included", 1, true)
        or message:find("IWYU:", 1, true) == 1) then
      diagnostics[#diagnostics + 1] = {
        lnum = tonumber(row) - 1,
        col = tonumber(col) - 1,
        severity = vim.diagnostic.severity.WARN,
        message = message,
        source = "iwyu",
        code = "missing-includes",
      }
    end
    local owner = line:match("^(.-) should remove these lines:$")
    if owner then
      removing = vim.fs.normalize(owner) == filename
    elseif line == "---" or line:find("The full include-list for ", 1, true) == 1
      or line:match(" should add these lines:$") then
      removing = false
    elseif removing then
      local include, first = line:match("^%- (#include.-)%s+// lines (%d+)%-%d+")
      if include then
        diagnostics[#diagnostics + 1] = {
          lnum = tonumber(first) - 1,
          col = 0,
          severity = vim.diagnostic.severity.WARN,
          message = "Unused include: " .. include,
          source = "iwyu",
          code = "unused-includes",
        }
      end
    end
  end
  return diagnostics
end

function M.spec()
  return {
    cmd = "python3",
    args = { root .. "/.toolchain/iwyu_check.py" },
    stdin = false,
    append_fname = true,
    stream = "stdout",
    ignore_exitcode = true,
    cwd = root,
    condition = function(ctx)
      return in_project(ctx.filename) and not vim.bo.modified
    end,
    parser = M.parse,
  }
end

function M.configure(opts)
  opts.linters = opts.linters or {}
  opts.linters.cp_iwyu = M.spec()
  opts.linters_by_ft = opts.linters_by_ft or {}
  for _, ft in ipairs({ "c", "cpp" }) do
    opts.linters_by_ft[ft] = opts.linters_by_ft[ft] or {}
    append_once(opts.linters_by_ft[ft], "cp_iwyu")
  end
end

function M.activate()
  local lint = require("lint")
  lint.linters.cp_iwyu = M.spec()
  for _, ft in ipairs({ "c", "cpp" }) do
    lint.linters_by_ft[ft] = lint.linters_by_ft[ft] or {}
    append_once(lint.linters_by_ft[ft], "cp_iwyu")
  end
  local filename = vim.api.nvim_buf_get_name(0)
  if in_project(filename) and not vim.bo.modified then
    lint.try_lint("cp_iwyu")
  end
  return true
end

return M
